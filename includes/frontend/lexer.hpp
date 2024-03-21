#ifndef W_LEXER_HPP
#define W_LEXER_HPP

#include <filesystem>
#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>
#include <cctype>
#include <memory>
#include <string>
#include <array>

namespace W {
    enum struct TokenKind {
        #define WLANG_TOKEN(X) X,

        #include <frontend/token_list.hpp>
    };

    struct Token {
        const std::filesystem::path& file_path;
        std::size_t line;
        std::size_t col;

        TokenKind kind;
        std::optional<std::string> data;
    };

    template <size_t N = 10>
    class Lexer {
    public:
        Lexer(const std::filesystem::path& path, std::istream& input);
        ~Lexer() = default;

        Token next();
        bool finished();

    private:
        bool start_with(std::string_view sv);
        char buffer_at(std::size_t i = 0);
        void advance(size_t offset = 1);
        void skip_whitespace();

        char parse_backslash();
        void read_ident(Token& token);
        void read_number(Token& token);
        void read_rune(Token& token);
        void read_string(Token& token, char open_quote);
        void read_comment(Token& token, bool is_multiline);

        const std::filesystem::path& m_path;
        std::array<char, N> m_working_buffer;
        std::istream& m_input;
        std::size_t m_line = 1;
        std::size_t m_col = 1;
    };
}

#include <frontend/lexer.inl>
#endif
