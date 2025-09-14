#ifndef W_LEXER_HPP
#define W_LEXER_HPP

#include <iostream>
#include <variant>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <array>
#include <fmt/core.h>

#include <utils/types.hpp>
#include <location.hpp>

namespace W {
    enum struct TokenKind {
        #define WLANG_TOKEN(X) X,

        #include <frontend/token_list.hpp>
    };

    struct Token {
        Location location;

        TokenKind kind;
        std::string raw;
    };

    const size_t LEXER_BUFFER_SIZE = 10;
    struct Lexer {
    public:
        Lexer(std::filesystem::path path, std::istream& input);
        Lexer(const Lexer&) = delete;
        Lexer(Lexer&&) noexcept = default;
        ~Lexer() = default;

        Lexer& operator=(const Lexer&) = delete;
        Lexer& operator=(Lexer&&) noexcept = default;

        Token next();
        bool finished();

    private:
        bool start_with(std::string_view sv);
        char buffer_at(std::size_t i = 0);
        void advance(size_t offset = 1);
        void skip_whitespace();

        void read_ident(Token& token);
        void read_number(Token& token);
        void read_string_or_rune(Token& token, char open_quote);
        void read_comment(bool is_multiline);

        std::shared_ptr<std::filesystem::path> m_path;
        std::array<char, LEXER_BUFFER_SIZE> m_working_buffer;
        std::istream& m_input;
        std::size_t m_line = 1;
        std::size_t m_col = 1;
    };
}

template <> struct fmt::formatter<W::TokenKind>: formatter<std::string_view> {
  // parse is inherited from formatter<std::string_view>.

  auto format(W::TokenKind kind, format_context& ctx) const
    -> format_context::iterator;
};

#endif
