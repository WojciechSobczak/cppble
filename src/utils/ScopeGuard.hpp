#pragma once
#include <functional>

namespace cppble
{

class ScopeGuard {
private:
    std::function<void()> destructor{};
    bool ignoreDestructor{false};
public:
    explicit ScopeGuard(const std::function<void()> &guard): destructor(guard) {};
    ~ScopeGuard() { if (!this->ignoreDestructor) { destructor(); }};

    void cancel() { this->ignoreDestructor = true; }
};

}

