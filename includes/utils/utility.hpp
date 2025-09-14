#ifndef UTILITY_H
#define UTILITY_H

#include <fmt/core.h>

namespace W::utils {
    [[noreturn]] inline void unreachable();
    template <typename... T>
    [[noreturn]] inline void panic(fmt::format_string<T...> fmt, T&&... args);
}

#include <utils/utility.inl>

#endif
