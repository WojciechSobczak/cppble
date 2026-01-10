#pragma once
#include "win_device.hpp"

#include <ppltasks.h>

#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

#include <semaphore>

#include "../utils/scope_guard.hpp"


namespace cppble
{

WinDevice::WinDevice(const WinAdvArgs& advertisementArgs) {
    //TODO: Make sure that this is the only way to retrieve name from adv
    this->_name = winrt::to_string(advertisementArgs.Advertisement().LocalName());
    this->_addressInt = advertisementArgs.BluetoothAddress();
    this->_addressStr = fmt::format("{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
        *(reinterpret_cast<const uint8_t*>(&this->_addressInt) + 5),
        *(reinterpret_cast<const uint8_t*>(&this->_addressInt) + 4),
        *(reinterpret_cast<const uint8_t*>(&this->_addressInt) + 3),
        *(reinterpret_cast<const uint8_t*>(&this->_addressInt) + 2),
        *(reinterpret_cast<const uint8_t*>(&this->_addressInt) + 1),
        *(reinterpret_cast<const uint8_t*>(&this->_addressInt) + 0)
    );

    //TODO: Make it correct with real bluetooth packet
    for (const WinAdvDataSection& section : advertisementArgs.Advertisement().DataSections()) {
        for (const uint8_t byte : std::span{section.Data().data(), section.Data().Length()}) {
            this->_advData.push_back(byte);
        }
    }
}

std::optional<error<device::connect_error>> WinDevice::connect_wait(const std::chrono::milliseconds timeout) {
    if (!this->_device.has_value()) {
        const auto asyncResult = WinNativeDevice::FromBluetoothAddressAsync(this->_addressInt);
        spdlog::debug("WinDevice::connect({}): WinNativeDevice requested", this->_addressStr);
        auto waitResult = asyncResult.Status();
        if (waitResult == WinAsyncStatus::Started) {
            waitResult = asyncResult.wait_for(timeout);
        } else {
            return error{connect_error::OS_ERROR, "Failed to start async device retrieval"};
        }

        switch (waitResult) {
            case WinAsyncStatus::Canceled: { return error{connect_error::OS_ERROR, "winrt::Windows::Foundation::IAsyncOperation cancelled"};}
            case WinAsyncStatus::Error: { return error{connect_error::OS_ERROR, "winrt::Windows::Foundation::IAsyncOperation error"};}
            case WinAsyncStatus::Started: { return error{connect_error::TIMEOUT, "Connection timeout"}; }
            case WinAsyncStatus::Completed: { break; }
        }

        auto device = asyncResult.GetResults();
        if (!device) {
            spdlog::debug("WinDevice::connect({}): Panic. Device nullptr", this->_addressStr);
            return error{connect_error::OS_ERROR, "CComPtr winrt::Windows::Devices::Bluetooth::BluetoothLEDevice is nullptr"};
        }
        spdlog::debug("WinDevice::connect({}): WinNativeDevice retrieved", this->_addressStr);
        this->_device = device;
    }

    if (this->_session.has_value()) {
        return error{connect_error::OS_ERROR, "WinGattSession already present. Its a bug. Should not happen."};
    }
    if (this->connected()) {
        return error{connect_error::ALREADY_CONNECTED, "Device already connected"};
    }

    {
        auto sessionCloser = scope_guard([this] {
            this->_connected.store(false);
            this->_session = {};
        });

        const auto asyncResult = WinGattSession::FromDeviceIdAsync(this->_device->BluetoothDeviceId());
        spdlog::debug("WinDevice::connect({}): WinGattSession requested", this->_addressStr);
        auto waitResult = asyncResult.Status();
        if (waitResult == WinAsyncStatus::Started) {
            waitResult = asyncResult.wait_for(timeout);
        } else {
            return error{connect_error::OS_ERROR, "Failed to start async device retrieval"};
        }

        switch (waitResult) {
            case WinAsyncStatus::Canceled: { return error{connect_error::OS_ERROR, "winrt::Windows::Foundation::IAsyncOperation cancelled"};}
            case WinAsyncStatus::Error: { return error{connect_error::OS_ERROR, "winrt::Windows::Foundation::IAsyncOperation error"};}
            case WinAsyncStatus::Started: { return error{connect_error::TIMEOUT, "Connection timeout"}; }
            case WinAsyncStatus::Completed: { break; }
        }

        auto session = asyncResult.GetResults();
        if (!session) {
            spdlog::debug("WinDevice::connect({}): Panic. Session nullptr.", this->_addressStr);
            return error{connect_error::OS_ERROR, "CComPtr winrt::Windows::Devices::Bluetooth::WinGattSession is nullptr"};
        }
        spdlog::debug("WinDevice::connect({}): WinGattSession retrieved", this->_addressStr);
        this->_session = session;

        try {
            this->_session->MaintainConnection(true);
        } catch (winrt::hresult_error& e) {
            return error{connect_error::OS_ERROR, fmt::format("WinGattSession::MaintainConnection(true) error: {}", winrt::to_string(e.message()))};
        }

        constexpr static auto onConnectionStatusChange = [](WinDevice& device, WinGattSessionStatus status) {
            spdlog::debug("{} connection status changed: {}", device._addressStr, status == WinGattSessionStatus::Active);
            device._connected.store(status == WinGattSessionStatus::Active);
        };

        spdlog::debug("WinDevice::connect({}): Waiting for session status change", this->_addressStr);
        std::binary_semaphore connectionWaiter(0);
        try {
            this->_session->SessionStatusChanged([this, &connectionWaiter](const WinGattSession&, const WinGattSessionStatusChangedEventArgs& args) {
                onConnectionStatusChange(*this, args.Status());
                connectionWaiter.release();
            });
        } catch (winrt::hresult_error& e) {
            return error{connect_error::OS_ERROR, fmt::format("WinGattSession::SessionStatusChanged() error: {}", winrt::to_string(e.message()))};
        }

        auto acquireResult = connectionWaiter.try_acquire_for(timeout);
        if (!acquireResult) {
            return error{connect_error::TIMEOUT, "Connection timeout"};
        }
        if (!this->connected()) {
            return error{connect_error::OS_ERROR, "WinGattSessionStatus::Status() Closed after connection attempt" };
        }
        try {
            this->_session->SessionStatusChanged([this](const WinGattSession&, const WinGattSessionStatusChangedEventArgs& args) {
                onConnectionStatusChange(*this, args.Status());
            });
        } catch (winrt::hresult_error& e) {
            return error{connect_error::OS_ERROR, fmt::format("WinGattSession::SessionStatusChanged() error: {}", winrt::to_string(e.message()))};
        }

        sessionCloser.cancel();
        spdlog::debug("{} connected", this->_addressStr);
    }
    return {};
}

void WinDevice::connect(const std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<connect_error>&)> onError) {
    Concurrency::create_task([this, timeout, onSuccess, onError]() {
        const auto error = this->connect_wait(timeout);
        if (error.has_value()) {
            onError(error.value());
        } else {
            onSuccess();
        }
    });
}

std::optional<error<device::disconnect_error>> WinDevice::disconnect_wait(const std::chrono::milliseconds timeout) {
    if (!this->_session.has_value() || !this->connected()) {
        spdlog::debug("WinDevice::disconnect_wait({}): already disconnected.", this->_addressStr);
        return error{disconnect_error::NOT_CONNECTED, "Device not connected"};
    }
    const auto sessionCloser = scope_guard([this] {
        this->_connected.store(false);
        this->_session = {};
    });
    try {
        this->_session->Close();
    } catch (winrt::hresult_error& e) {
        return error{disconnect_error::OS_ERROR, fmt::format("WinGattSession::Close() error: {}", winrt::to_string(e.message()))};
    }
    spdlog::debug("WinDevice::disconnect_wait({}): session closed.", this->_addressStr);
    return {};
}

void WinDevice::disconnect(const std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<disconnect_error>&)> onError) {
    Concurrency::create_task([this, timeout, onSuccess, onError]() {
        const auto error = this->disconnect_wait(timeout);
        if (error.has_value()) {
            onError(error.value());
        } else {
            onSuccess();
        }
    });
}


}
