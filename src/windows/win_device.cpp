#pragma once
#include "win_device.hpp"

#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"


namespace cppble
{

WinDevice::WinDevice(const WinAdvArgs& advertisementArgs) {
    this->_name = winrt::to_string(advertisementArgs.Advertisement().LocalName());

    const uint64_t uintAddress = advertisementArgs.BluetoothAddress();
    this->_address = fmt::format("{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
        *(reinterpret_cast<const uint8_t*>(&uintAddress) + 5),
        *(reinterpret_cast<const uint8_t*>(&uintAddress) + 4),
        *(reinterpret_cast<const uint8_t*>(&uintAddress) + 3),
        *(reinterpret_cast<const uint8_t*>(&uintAddress) + 2),
        *(reinterpret_cast<const uint8_t*>(&uintAddress) + 1),
        *(reinterpret_cast<const uint8_t*>(&uintAddress) + 0)
    );

    //TODO: Make it correct with real bluetooth packet
    for (const WinAdvDataSection& section : advertisementArgs.Advertisement().DataSections()) {
        for (const uint8_t byte : std::span{section.Data().data(), section.Data().Length()}) {
            this->_adv_data.push_back(byte);
        }
    }
}


}
