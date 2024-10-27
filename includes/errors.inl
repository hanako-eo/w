#include <string>
#include <tuple>
#include <fmt/format.h>

namespace W {
    #define WLANG_ERROR(error_name, error_message, ...) \
        template<typename... Args> \
        inline error_name##Error::error_name##Error(Location location, Args&&... args): \
            Exception(std::move(location), ExceptionType::error_name), \
            m_parameters(std::forward<Args>(args)...) \
        {} \
        \
        inline std::string error_name##Error::generate_error_message() const { \
            return std::apply([&](const auto... args) { return fmt::format(error_message, args...); }, m_parameters); \
        }

    #include <error_list.hpp>
}
