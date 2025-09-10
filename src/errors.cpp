#include <errors.hpp>

namespace W {
    const std::string& Exception::get_message() {
        if (m_message.empty())
            m_message = generate_error_message();

        return m_message;
    }
}