#pragma once
#include <functional>
#include <memory>

#include "device.hpp"

namespace cppble
{

class scanner {
public:
    virtual ~scanner() = default;

    static std::unique_ptr<scanner> create();

    virtual void start(std::function<void(std::unique_ptr<device>)> onDeviceAppear) = 0;
    virtual void stop() = 0;
};


}
