#include <algorithm>
#include <utility>
#include <optional>
#include <string>
#include <memory>

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <utils/macros.hpp>

namespace W {
    constexpr auto s_reserved_keywords = frozen::make_unordered_map<frozen::string, TokenKind>({
        { "asm", TokenKind::KeyAsm },
        { "assert", TokenKind::KeyAssert },
        { "as", TokenKind::KeyAs },
        { "atomic", TokenKind::KeyAtomic },
        { "break", TokenKind::KeyBreak },
        { "const", TokenKind::KeyConst },
        { "continue", TokenKind::KeyContinue },
        { "defer", TokenKind::KeyDefer },
        { "else", TokenKind::KeyElse },
        { "enum", TokenKind::KeyEnum },
        { "false", TokenKind::KeyFalse },
        { "for", TokenKind::KeyFor },
        { "fn", TokenKind::KeyFn },
        { "global", TokenKind::KeyGlobal },
        { "goto", TokenKind::KeyGoto },
        { "if", TokenKind::KeyIf },
        { "import", TokenKind::KeyImport },
        { "interface", TokenKind::KeyInterface },
        { "in", TokenKind::KeyIn },
        { "is", TokenKind::KeyIs },
        { "match", TokenKind::KeyMatch },
        { "module", TokenKind::KeyModule },
        { "mut", TokenKind::KeyMut },
        { "nil", TokenKind::KeyNil },
        { "none", TokenKind::KeyNone },
        { "return", TokenKind::KeyReturn },
        { "sizeof", TokenKind::KeySizeof },
        { "likely", TokenKind::KeyLikely },
        { "like", TokenKind::KeyLike },
        { "unlikely", TokenKind::KeyUnlikely },
        { "offsetof", TokenKind::KeyOffsetof },
        { "struct", TokenKind::KeyStruct },
        { "true", TokenKind::KeyTrue },
        { "typeof", TokenKind::KeyTypeof },
        { "type", TokenKind::KeyType },
        { "dump", TokenKind::KeyDump },
        { "or", TokenKind::KeyOr },
        { "union", TokenKind::KeyUnion },
        { "pub", TokenKind::KeyPub },
        { "static", TokenKind::KeyStatic },
        { "volatile", TokenKind::KeyVolatile },
        { "unsafe", TokenKind::KeyUnsafe },
    });

    constexpr auto s_operations = frozen::make_unordered_map<frozen::string, TokenKind>({
        { ">>>=", TokenKind::UnsignedRightShiftAssign },
        { ">>>", TokenKind::UnsignedRightShift },
        { "<<=", TokenKind::RightShiftAssign },
        { "<<", TokenKind::LeftShift },
        { ">>=", TokenKind::LeftShiftAssign },
        { ">>", TokenKind::RightShift },
        { ">=", TokenKind::Ge },
        { ">", TokenKind::Gt },
        { "<=", TokenKind::Le },
        { "<", TokenKind::Lt },
        { "==", TokenKind::Eq },
        { "!=", TokenKind::Ne },
        { ":=", TokenKind::DeclAssign },
        { "+=", TokenKind::PlusAssign },
        { "-=", TokenKind::MinusAssign },
        { "/=", TokenKind::DivAssign },
        { "*=", TokenKind::MultAssign },
        { "^=", TokenKind::XorAssign },
        { "%=", TokenKind::ModAssign },
        { "|=", TokenKind::OrAssign },
        { "&=", TokenKind::AndAssign },
        { "=", TokenKind::Assign },
        { "!in", TokenKind::NotIn },
        { "!is", TokenKind::NotIs },
        { "!", TokenKind::Not },
        { "++", TokenKind::Inc },
        { "+", TokenKind::Plus },
        { "--", TokenKind::Dec },
        { "-", TokenKind::Minus },
        { "*", TokenKind::Mul },
        { "/", TokenKind::Div },
        { "%", TokenKind::Mod },
        { "^", TokenKind::Xor },
        { "~", TokenKind::BitNot },
        { "?", TokenKind::Question },
        { ",", TokenKind::Comma },
        { ";", TokenKind::Semicolon },
        { ":", TokenKind::Colon },
        { "&&", TokenKind::And },
        { "&", TokenKind::Amp },
        { "||", TokenKind::LogicalOr },
        { "|", TokenKind::Pipe },
        { "@", TokenKind::At },
        { "{", TokenKind::Lcbr },
        { "}", TokenKind::Rcbr },
        { "(", TokenKind::Lpar },
        { ")", TokenKind::Rpar },
        { "#[", TokenKind::Nilsbr },
        { "[", TokenKind::Lsbr },
        { "]", TokenKind::Rsbr },
        { "#", TokenKind::Hash },
        { "...", TokenKind::Ellipsis },
        { "..", TokenKind::Dotdot },
        { ".", TokenKind::Dot },
    });

    template <size_t N>
    Lexer<N>::Lexer(const std::filesystem::path& path, std::istream& input): 
        m_path(path),
        m_input(input)
    {
        for (size_t i = 0; i < N; i++) {
            int32_t c = m_input.get();
            m_working_buffer[i] = c != EOF ? (char)c : 0;
        }
    }

    template <size_t N>
    char Lexer<N>::buffer_at(std::size_t i) {
        return m_working_buffer.at(i);
    }

    template <size_t N>
    bool Lexer<N>::start_with(std::string_view x) {
        return std::string_view(m_working_buffer.begin(), m_working_buffer.begin() + x.size()) == x;
    }

    template <size_t N>
    void Lexer<N>::advance(size_t offset) {
        // shift the whole buffer by offset
        std::shift_left(m_working_buffer.begin(), m_working_buffer.end(), offset);

        for (size_t i = 0; i < offset; i++) {
            int32_t c = m_input.get();
            m_working_buffer[N - offset + i] = c != EOF ? (char)c : 0;
            if (c == '\n') {
                m_line += 1;
                m_col = 0;
            } else {
                m_col += 1;
            }
        }
    }

    template <size_t N>
    void Lexer<N>::skip_whitespace() {
        while (std::isspace(buffer_at()) && !finished())
            advance();
    }

    template <size_t N>
    bool Lexer<N>::finished() {
        return m_working_buffer.at(0) == 0;
    }

    template <size_t N>
    Token Lexer<N>::next() {
        skip_whitespace();

        Token token = {m_path, m_line, m_col, TokenKind::Unknown, std::nullopt};

        if (finished()) {
            token.kind = TokenKind::Eof;
            return token;
        }

        char c = buffer_at();

        if (std::isalpha(c) || c == '_')
            read_ident(token);
        else if (std::isdigit(c))
            read_number(token);
        else if (c == '`')
            read_rune(token);
        else if (c == '"' || c == '\'')
            read_string(token, c);
        else if (start_with("//") || start_with("/*"))
            read_comment(token, buffer_at(1) == '*');
        else {
            for (auto op : s_operations) {
                if (start_with(op.first.data())) {
                    token.kind = op.second;
                    advance(op.first.size());
                    break;
                }
            }
        }

        return token;
    }

    template <size_t N>
    void Lexer<N>::read_ident(Token& token) {
        char c = buffer_at();
        std::string data;

        while (std::isalnum(c) || c == '_') {
            data.push_back(c);
            advance();
            c = buffer_at();
        }

        if (auto it = s_reserved_keywords.find(std::string_view(data)); it == s_reserved_keywords.end()) {
            token.kind = TokenKind::Ident;
            token.data = data;
        } else
            token.kind = it->second;
    }

    template <size_t N>
    void Lexer<N>::read_number(Token& token) {
        char c = buffer_at();
        std::string data;

        while (isxdigit(c) || c == '_') {
            // TODO: error: this number has unsuitable digit
            data.push_back(c);
            advance();
            c = buffer_at();
        }

        token.kind = TokenKind::Number;
        token.data = data;
    }

    template <size_t N>
    void Lexer<N>::read_rune(Token& token) {
        std::string data;

        // skip start '`'
        advance();
        char c = buffer_at();
        if (c == '\\') {
            c = parse_backslash();
            if (c > -1) data.push_back(c);
            else todo("error \\n in char")
            advance();
            c = buffer_at();
        } else {
            // parse utf-8 char
            int len = 0;

            // test 0xxxxxxx
            if (c >> 7 == 0) len = 1;
            // test 110xxxxx
            else if (c >> 5 == 0x6) len = 2;
            // test 1110xxxx
            else if (c >> 4 == 0xE) len = 3;
            // test 11110xxx
            else if (c >> 5 == 0x1E) len = 4;
            // get only the first byte invalid
            else len = 1;

            while (len--) {
                if (c == '`') break;

                data.push_back(c);
                advance();
                c = buffer_at();
            }
        }

        // skip end '`'
        if (c == '`') advance();
        else todo("error for '`' at the end of rune");

        token.kind = TokenKind::Rune;
        token.data = data;
    }

    template <size_t N>
    void Lexer<N>::read_comment(Token& token, bool is_multiline) {
        char c = buffer_at();
        std::string data;

        advance(2);

        if (is_multiline) {
            int32_t nesting = 0;
            while (c != 0) {
                if (start_with("*/")) {
                    if (!nesting) break;
                    nesting--;
                } else if (start_with("/*")) nesting++;
                data.push_back(c);
                c = buffer_at();
            }
        } else {
            while (c != '\n') {
                data.push_back(c);
                c = buffer_at();
            }
        }
        
        token.kind = TokenKind::Comment;
        token.data = data;
    }

    template <size_t N>
    char Lexer<N>::parse_backslash() {
        // skip '\'
        advance();
        char c = buffer_at();
    
        switch (c) {
        case '0': {
            return '\0';
        }
        case 'n': {
            return '\n';
        }
        case 'r': {
            return '\r';
        }
        case 't': {
            return '\t';
        }
        case 'b': {
            return '\b';
        }
        case 'f': {
            return '\f';
        }
        case 'v': {
            return '\v';
        }
        case 'x': {
            advance();
            char a = buffer_at();
            if (!isxdigit(a)) todo("error for \\x");

            advance();
            char b = buffer_at();
            if (!isxdigit(b)) todo("error for \\x");

            a = (a <= '9') ? a - '0' : (a & 0x7) + 9;
            b = (b <= '9') ? b - '0' : (b & 0x7) + 9;

            return (a << 4) + b;
        }
        case '\n': {
            return -1;
        }
        default: {
            return c;
        }
        }
    }

    template <size_t N>
    void Lexer<N>::read_string(Token& token, char open_quote) {
        std::string data;
        // skip open " or '
        advance();

        char c = buffer_at();
        while (c > 0 && c != open_quote) {
            if (c == '\\') {
                c = parse_backslash();
                if (c > -1) data.push_back(c);
            }
            else data.push_back(c);
            advance();
            c = buffer_at();
        }

        // skip end " or '
        if (c == open_quote) advance();
        else todof("error to find `%c` at the end of rune", open_quote);
        
        token.kind = TokenKind::String;
        token.data = data;
    }
}
