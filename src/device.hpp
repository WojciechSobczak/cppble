#pragma once
#include <string_view>
#include <span>
#include <chrono>
#include <functional>

#include "error.hpp"

namespace cppble
{

class device {
public:
    virtual ~device() = default;

    [[nodiscard]] virtual std::string_view name() const = 0;
    [[nodiscard]] virtual std::string_view address() const = 0;
    [[nodiscard]] virtual std::span<const uint8_t> advertisement_bytes() const = 0;
    [[nodiscard]] virtual bool connected() const = 0;
    [[nodiscard]] virtual bool paired() const = 0;

    enum class connect_error {
        OS_ERROR,
        ALREADY_CONNECTED,
        TIMEOUT
    };
    [[nodiscard]] virtual std::optional<error<connect_error>> connect_wait(std::chrono::milliseconds timeout) = 0;
    virtual void connect(std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<connect_error>&)> onError) = 0;

    enum class disconnect_error {
        OS_ERROR,
        NOT_CONNECTED,
        TIMEOUT
    };
    [[nodiscard]] virtual std::optional<error<disconnect_error>> disconnect_wait(std::chrono::milliseconds timeout) = 0;
    virtual void disconnect(std::chrono::milliseconds timeout, std::function<void()> onSuccess, std::function<void(const error<disconnect_error>&)> onError) = 0;
};

}
