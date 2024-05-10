#include <fmt/format.h>
#include <errors.hpp>

namespace W {
    Exception::Exception(Location location, ExceptionType type):
        m_location(std::move(location)),
        m_type(type)
    {}

    const std::string& Exception::get_message() {
        if (m_message.empty())
            m_message = generate_error_message();

        return m_message;
    }

    const ExceptionType Exception::get_type() const {
        return m_type;
    }

    inline const Location& Exception::get_location() const {
        return m_location;
    }

    const char* Exception::what() {
        return get_message().c_str();
    }
}