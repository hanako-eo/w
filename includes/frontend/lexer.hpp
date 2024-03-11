#ifndef W_LEXER_HPP
#define W_LEXER_HPP

#include <filesystem>
#include <iostream>
#include <optional>
#include <fstream>
#include <cctype>
#include <memory>
#include <string>

namespace W {
    enum struct TokenKind {
        #define WLANG_TOKEN(X) X,

        #include <frontend/token_list.hpp>
    };

    struct Token {
        std::shared_ptr<const std::filesystem::path> file_path;
        std::size_t line;
        std::size_t col;

        TokenKind kind;
        std::optional<std::string> data;
    };

    class Lexer {
    public:
        Lexer(std::filesystem::path path);
        ~Lexer() = default;

        Token next();

    private:
        bool start_with(std::string_view sv);
        char buffer_at(std::size_t i = 0);
        void advance();
        void skip_whitespace();

        char parse_backslash();
        void read_ident(Token& token);
        void read_number(Token& token);
        void read_rune(Token& token);
        void read_string(Token& token, char open_quote);
        void read_comment(Token& token, bool is_multiline);

        std::shared_ptr<const std::filesystem::path> m_path;
        std::string m_current_line;
        std::ifstream m_file;
        std::size_t m_line = 1;
        std::size_t m_col = 1;
    };
}

#endif
