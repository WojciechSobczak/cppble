#pragma once
#include <string>
#include <vector>

#include "winrt_includes.hpp"
#include "../device.hpp"
#include "../error.hpp"


namespace cppble
{

class WinDevice : public device {
private:
    uint64_t _addressInt{};
    std::string _addressStr{};
    std::string _name{};
    std::vector<uint8_t> _advData{};

    std::optional<WinNativeDevice> _device{};
    std::optional<WinGattSession> _session{};
    bool _connected{false};
    bool _paired{false};

public:
    explicit WinDevice(const WinAdvArgs& advertisementArgs);

    [[nodiscard]] std::string_view name() const override { return _name; }
    [[nodiscard]] std::string_view address() const override { return _addressStr; }
    [[nodiscard]] std::span<const uint8_t> advertisement_bytes() const override { return _advData; }
    [[nodiscard]] bool connected() const override { return _connected; }
    [[nodiscard]] bool paired() const override { return _paired; }

    [[nodiscard]] std::optional<error<connect_error>> connect_wait(std::chrono::milliseconds timeout) override;
    void connect(std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<connect_error>&)> onError) override;

    [[nodiscard]] std::optional<error<disconnect_error>> disconnect_wait() override;
    void disconnect() override;

};


}
