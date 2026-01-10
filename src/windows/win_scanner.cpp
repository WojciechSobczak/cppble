#include "win_scanner.hpp"
#include "winrt_includes.hpp"
#include "win_device.hpp"

namespace cppble
{

//TODO: Check with linux api if scanning modes are available and pass is as parameter
void WinScanner::start(std::function<void(std::unique_ptr<device>)> onDeviceAppear) {
    watcher.ScanningMode(WinRTLowEnergyScaningMode::Active);
    watcher.Received([onDeviceAppear](const WinAdvWatcher&, const WinAdvArgs& args) {
        onDeviceAppear(std::make_unique<WinDevice>(args));
    });
    watcher.Start();
}

void WinScanner::stop() {
    watcher.Stop();
}

}
