#pragma once

#include <string>

namespace Infinity {
    template<typename T>
    std::string get_type_name() {
#ifdef __clang__
        std::string name = __PRETTY_FUNCTION__;
        auto start = name.find('=') + 2;
        auto end = name.find(']');
        return name.substr(start, end - start);
#elif defined(__GNUC__)
        std::string name = __PRETTY_FUNCTION__;
        auto start = name.find("with T = ") + 9;
        auto end = name.find(';', start);
        return name.substr(start, end - start);
#elif defined(_MSC_VER)
        std::string name = __FUNCSIG__;
        auto start = name.find("get_type_name<") + 14;
        auto end = name.find(">(void)");
        return name.substr(start, end - start);
#else
        return "Unsupported compiler";
#endif
    }

    inline std::string get_arg_types() { return "void"; }

    template<typename First, typename... Rest>
    std::string get_arg_types() {
        std::string args = get_type_name<First>();
        if constexpr (sizeof...(Rest) > 0) {
            args += ", " + get_arg_types<Rest...>();
        }
        return args;
    }

    template<typename Ret, typename... Args>
    std::string get_function_signature(const std::string &func_name) {
        return get_type_name<Ret>() + " " + func_name + "(" + get_arg_types<Args...>() + ")";
    }
} // namespace Infinity
