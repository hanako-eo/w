#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
#include <string>
#include <tuple>

#include <location.hpp>
#include <frontend/lexer.hpp>

namespace W {
    enum struct ExceptionType {
        #define WLANG_ERROR(error_name, ...) error_name,

        #include <error_list.hpp>
    };

    struct Exception : public std::exception {
        public:
            inline Exception(Location location, ExceptionType type) noexcept;
            Exception(const Exception&) = default;
            Exception(Exception&&) noexcept = default;
            ~Exception() noexcept = default;

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
        struct error_name##Error : public Exception { \
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
