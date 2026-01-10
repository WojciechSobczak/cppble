#pragma once
#include <string>

namespace cppble
{

template<typename T>
concept error_code_enum = requires(T t)
{
    { std::is_scoped_enum_v<T> };
};

template <typename error_code>
    requires error_code_enum<error_code>
class error {
private:
    error_code _code;
    std::string _description;
public:
    explicit error(const error_code code): _code(code) {}
    explicit error(const error_code code, const std::string &description): _code(code), _description(description) {}

    [[nodiscard]] error_code code() const { return _code; };
    [[nodiscard]] std::string_view description() const { return _description; };
};


}