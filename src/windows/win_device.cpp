#pragma once
#include "win_device.hpp"

#include <ppltasks.h>

#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"


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
    {
        const auto asyncResult = WinNativeDevice::FromBluetoothAddressAsync(this->_addressInt);
        spdlog::debug("WinDevice::connect(): Device requested. Address: {}", this->_addressStr);
        auto waitResult = asyncResult.Status();
        if (asyncResult.Status() == WinAsyncStatus::Started) {
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
            spdlog::debug("WinDevice::connect(): Panic. Device nullptr. Address: {}", this->_addressStr);
            return error{connect_error::OS_ERROR, "CComPtr winrt::Windows::Devices::Bluetooth::BluetoothLEDevice is nullptr"};
        }
        spdlog::debug("WinDevice::connect(): Device retrieved. Address: {}", this->_addressStr);
        this->_device = device;
    }


    {
        const auto asyncResult = WinGattSession::FromDeviceIdAsync(this->_device->BluetoothDeviceId());
        spdlog::debug("WinDevice::connect(): Session requested. Address: {}", this->_addressStr);
        auto waitResult = asyncResult.Status();
        if (asyncResult.Status() == WinAsyncStatus::Started) {
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
            spdlog::debug("WinDevice::connect(): Panic. Session nullptr. Address: {}", this->_addressStr);
            return error{connect_error::OS_ERROR, "CComPtr winrt::Windows::Devices::Bluetooth::WinGattSession is nullptr"};
        }
        spdlog::debug("WinDevice::connect(): Session retrieved. Address: {}", this->_addressStr);
        this->_session = session;
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

std::optional<error<device::disconnect_error>> WinDevice::disconnect_wait() {
    return {};
}

void WinDevice::disconnect() {

}


}
