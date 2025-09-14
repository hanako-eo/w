#include <cpptrace/cpptrace.hpp>
#include <fmt/core.h>

namespace W::utils {
    [[noreturn]] inline void unreachable() {
        #if defined(DEBUG)
            cpptrace::generate_trace().print();
        #endif
        // Uses compiler specific extensions if possible.
        // Even if no extension is used, undefined behavior is still raised by
        // an empty function body and the noreturn attribute.
        #if defined(_MSC_VER) && !defined(__clang__) // MSVC
            __assume(false);
        #else // GCC, Clang
            __builtin_unreachable();
        #endif
    }

    template <typename... T>
    [[noreturn]] inline void panic(fmt::format_string<T...> fmt, T&&... args) {
        fmt::print("Program panic: {}\n", fmt::format(fmt, args...));
        #if defined(DEBUG)
            // removes the panic call from the trace
            cpptrace::generate_trace(1).print();
        #endif

        std::abort();
    }
}
