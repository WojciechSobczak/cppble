#include <map>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

#include "scanner.hpp"

int main() {
    spdlog::set_level(spdlog::level::trace);

    const auto scanner = cppble::scanner::create();
    auto devices = std::map<std::string, std::unique_ptr<cppble::device>>();
    scanner->start([&devices](std::unique_ptr<cppble::device> device) {
        spdlog::debug("[Device] Name: {} | Address: {} | Data: {:nsp}",
            device->name(),
            device->address(),
            spdlog::to_hex(device->advertisement_bytes())
        );
        devices.insert({std::string(device->name()), std::move(device)});
    });

    while (devices.size() != 2) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
