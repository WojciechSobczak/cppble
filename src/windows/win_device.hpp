#pragma once
#include <string>
#include <vector>

#include "winrt_includes.hpp"
#include "../device.hpp"


namespace cppble
{

class WinDevice : public device {
private:
    std::string _address{};
    std::string _name{};
    std::vector<uint8_t> _adv_data{};

public:
    explicit WinDevice(const WinAdvArgs& advertisementArgs);

    const std::string_view name() const override { return _name; }
    const std::string_view address() const override { return _address; }
    const std::span<const uint8_t> advertisement_bytes() const override { return _adv_data; }
};


}
