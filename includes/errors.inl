#include <string>
#include <tuple>
#include <fmt/format.h>

namespace W {
    inline Exception::Exception(Location location, ExceptionType type) noexcept :
        m_location(std::move(location)),
        m_type(type)
    {}

    inline const ExceptionType Exception::get_type() const {
        return m_type;
    }

    inline const Location& Exception::get_location() const {
        return m_location;
    }

    inline const char* Exception::what() {
        return get_message().c_str();
    }

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
