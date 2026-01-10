#pragma once
#include <string_view>
#include <span>

namespace cppble
{

class device {
public:
    virtual ~device() = default;

    virtual const std::string_view name() const = 0;
    virtual const std::string_view address() const = 0;
    virtual const std::span<const uint8_t> advertisement_bytes() const = 0;
};

}
