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
    bool _paired{false};
    uint16_t _mtu{DEFAULT_MTU};
    std::function<void(uint16_t)> _on_mtu_changed{};
    std::function<void()> _on_connected{};
    std::function<void()> _on_disconnected{};

public:
    explicit WinDevice(const WinAdvArgs& advertisementArgs);

    [[nodiscard]] std::string_view name() const override { return _name; }
    [[nodiscard]] std::string_view address() const override { return _addressStr; }
    [[nodiscard]] std::span<const uint8_t> advertisement_bytes() const override { return _advData; }
    [[nodiscard]] bool connected() const override { return _session.has_value() ? _session->SessionStatus() == WinGattSessionStatus::Active : false; }
    [[nodiscard]] bool paired() const override { return _paired; }
    [[nodiscard]] uint16_t mtu() const override { return _session.has_value() ? _session->MaxPduSize() : DEFAULT_MTU; }

    void on_mtu_changed(const std::function<void(uint16_t)> on_mtu_changed) override { this->_on_mtu_changed = on_mtu_changed; };
    void on_connected(const std::function<void()> on_connected) override { this->_on_connected = on_connected; };
    void on_disconnected(const std::function<void()> on_disconnected) override { this->_on_disconnected = on_disconnected; };

    [[nodiscard]] std::optional<error<connect_error>> connect_wait(std::chrono::milliseconds timeout) override;
    void connect(std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<connect_error>&)> onError) override;

    [[nodiscard]] std::optional<error<disconnect_error>> disconnect_wait(std::chrono::milliseconds timeout) override;
    void disconnect(std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<disconnect_error>&)> onError) override;

};


}
