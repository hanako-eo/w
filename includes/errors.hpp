#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
#include <string>
#include <tuple>
#include <fmt/core.h>

#include <location.hpp>
#include <frontend/lexer.hpp>

template <> struct fmt::formatter<W::TokenKind>: formatter<std::string_view> {
  // parse is inherited from formatter<std::string_view>.

  auto format(W::TokenKind kind, format_context& ctx) const
    -> format_context::iterator;
};


namespace W {
    enum struct ExceptionType {
        #define WLANG_ERROR(error_name, ...) error_name,

        #include <error_list.hpp>
    };

    class Exception : public std::exception {
        public:
            Exception(Location location, ExceptionType type);
            Exception(const Exception&) = default;
            Exception(Exception&&) noexcept = default;
            ~Exception() = default;

            const std::string& get_message();
            inline const ExceptionType get_type() const;
            inline const Location& get_location() const;
            inline const char* what();
        
        protected:
            virtual std::string generate_error_message() const = 0;

        private:
            std::string m_message;
            Location m_location;
            ExceptionType m_type;
    };

    #define WLANG_ERROR(error_name, error_message, ...) \
        class error_name##Error : public Exception { \
            public: \
                template<typename... Args>\
                inline error_name##Error(Location location, Args&&... args); \
            private: \
                std::string generate_error_message() const override;\
                std::tuple<__VA_ARGS__> m_parameters; \
        };

    #include <error_list.hpp>
}

#include <errors.inl>

#endif
