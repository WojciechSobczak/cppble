#include "win_adapter.hpp"
#include "winrt_includes.hpp"

#include <fmt/format.h>
#include <semaphore>
#include <spdlog/spdlog.h>

namespace cppble
{


// std::vector<WinAdapter> WinAdapter::listAdapters() {
//     spdlog::trace("WinAdapter::listAdapters()");
//
//     auto watcher = WinDeviceInformation::CreateWatcher();
//     std::binary_semaphore semaphore(0);
//
//
//     std::vector<WinDeviceInformation> adapters;
//     watcher.Added([&adapters](const WinDeviceWatcher&, const WinDeviceInformation& info) {
//         spdlog::trace("WinDeviceInformation::Watcher::Added(): {}", winrt::to_string(info.Name()));
//         adapters.emplace_back(info);
//     });
//     watcher.Updated([&adapters](const WinDeviceWatcher&, const WinDeviceInformationUpdate&) {
//         spdlog::trace("WinDeviceInformation::Watcher::Updated()");
//     });
//     watcher.EnumerationCompleted([&semaphore](const WinDeviceWatcher&, const WinRTInspectable& inspectable) {
//         spdlog::trace("WinDeviceInformation::Watcher::EnumerationCompleted()");
//         semaphore.release();
//     });
//     watcher.Stopped([&semaphore](const WinDeviceWatcher&, const WinRTInspectable&) {
//         spdlog::trace("WinDeviceInformation::Watcher::Stopped()");
//         semaphore.release();
//     });
//     watcher.Removed([&adapters](const WinDeviceWatcher&, const WinRTInspectable&) {
//         spdlog::trace("WinDeviceInformation::Watcher::Removed()");
//     });
//
//
//     watcher.Start();
//     semaphore.acquire();
//
//     for (auto a : adapters) {
//         spdlog::trace("Filtered: {}", winrt::to_string(a.Name()));
//     }
//
//     return {};
// }


}
