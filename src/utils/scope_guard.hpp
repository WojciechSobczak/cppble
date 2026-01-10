#pragma once
#include <functional>

namespace cppble
{

class scope_guard {
private:
    std::function<void()> destructor{};
    bool ignoreDestructor{false};
public:
    explicit scope_guard(const std::function<void()> &guard): destructor(guard) {};
    ~scope_guard() { if (!this->ignoreDestructor) { destructor(); }};

    void cancel() { this->ignoreDestructor = true; }
};

}

