#pragma once
#include "winrt_includes.hpp"
#include "../scanner.hpp"

namespace cppble
{

class WinScanner : public scanner {
private:
    WinRTLowEnergyAdvDataWatcher watcher;
public:
    ~WinScanner() override = default;

    void start(std::function<void(std::unique_ptr<device>)> onDeviceAppear) override;
    void stop() override;
};

}
