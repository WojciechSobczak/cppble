#include <map>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <semaphore>

#include "cppble.hpp"


using namespace std::chrono_literals;

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

    while (devices.find("Redmi-Nice") == devices.end()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::binary_semaphore waiter(0);

    auto& phone = devices.at("Redmi-Nice");
    phone->connect(5s, [&waiter] {
        spdlog::debug("Connected");
        waiter.release();
    }, [&waiter](const cppble::error<cppble::device::connect_error>& error) {
        spdlog::debug("Error code: {}, Error desc: {}", (int) error.code(), error.description());
        waiter.release();
    });
    waiter.acquire();
}
