#include <algorithm>
#include <utility>
#include <optional>
#include <string>
#include <memory>

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <frontend/lexer.hpp>
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
        { "^", TokenKind::BitXor },
        { "~", TokenKind::BitNot },
        { "?", TokenKind::Question },
        { ",", TokenKind::Comma },
        { ";", TokenKind::Semicolon },
        { ":", TokenKind::Colon },
        { "&&", TokenKind::LogicalAnd },
        { "&", TokenKind::BitAnd },
        { "||", TokenKind::LogicalOr },
        { "|", TokenKind::BitOr },
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

    Lexer::Lexer(std::filesystem::path path, std::istream& input): 
        m_path(std::make_shared<std::filesystem::path>(std::move(path))),
        m_input(input)
    {
        for (size_t i = 0; i < LEXER_BUFFER_SIZE; i++) {
            int32_t c = m_input.get();
            m_working_buffer[i] = c != EOF ? (char)c : 0;
        }
    }

    char Lexer::buffer_at(std::size_t i) {
        return m_working_buffer.at(i);
    }

    bool Lexer::start_with(std::string_view x) {
        return std::string_view(m_working_buffer.begin(), m_working_buffer.begin() + x.size()) == x;
    }

    void Lexer::advance(size_t offset) {
        // shift the whole buffer by offset
        std::shift_left(m_working_buffer.begin(), m_working_buffer.end(), offset);

        for (size_t i = 0; i < offset; i++) {
            int32_t c = m_input.get();
            m_working_buffer[LEXER_BUFFER_SIZE - offset + i] = c != EOF ? (char)c : 0;
            if (c == '\n') {
                m_line += 1;
                m_col = 0;
            } else {
                m_col += 1;
            }
        }
    }

    void Lexer::skip_whitespace() {
        while (std::isspace(buffer_at()) && !finished())
            advance();
    }

    bool Lexer::finished() {
        return m_working_buffer.at(0) == 0;
    }

    Token Lexer::next() {
        skip_whitespace();
        while (start_with("//") || start_with("/*")){
            read_comment(buffer_at(1) == '*');
            skip_whitespace();
        }

        Token token = {Location{m_path, m_line, m_col, 0, 0}, TokenKind::Unknown, {}};

        if (finished()) {
            token.kind = TokenKind::Eof;
            return token;
        }

        char c = buffer_at();

        if (std::isalpha(c) || c == '_')
            read_ident(token);
        else if (std::isdigit(c))
            read_number(token);
        else if (c == '"' || c == '\'' || c == '`')
            read_string_or_rune(token, c);
        else {
            for (auto op : s_operations) {
                if (start_with(op.first.data())) {
                    token.kind = op.second;
                    advance(op.first.size());
                    break;
                }
            }
        }

        token.location.end_line = m_line;
        token.location.end_col = m_col;

        return token;
    }

    void Lexer::read_ident(Token& token) {
        char c = buffer_at();
        std::string data;

        while (std::isalnum(c) || c == '_') {
            data.push_back(c);
            advance();
            c = buffer_at();
        }

        if (auto it = s_reserved_keywords.find(std::string_view(data)); it == s_reserved_keywords.end()) {
            token.kind = TokenKind::Ident;
            token.raw = data;
        } else
            token.kind = it->second;
    }

    void Lexer::read_number(Token& token) {
        char c = buffer_at();
        std::string raw;

        while (isxdigit(c) || c == '_') {
            raw.push_back(c);
            advance();
            c = buffer_at();
        }

        if (c == '.') {
            do {
                raw.push_back(c);
                advance();
                c = buffer_at();
            } while (isxdigit(c) || c == '_');

            token.kind = TokenKind::Float;
        } else {
            token.kind = TokenKind::Integer;
        }
        token.raw = raw;
    }

    void Lexer::read_comment(bool is_multiline) {
        char c = buffer_at();
        advance(2);

        if (is_multiline) {
            int32_t nesting = 0;
            while (c != 0) {
                if (start_with("*/")) {
                    if (!nesting) break;
                    nesting--;
                } else if (start_with("/*")) nesting++;
                advance();
                c = buffer_at();
            }
            // skiping */
            advance(2);
        } else {
            while (c != '\n') {
                advance();
                c = buffer_at();
            }
        }
    }

    void Lexer::read_string_or_rune(Token& token, char open_quote) {
        std::string data;
        // skip open ", ' or `
        advance();

        char c = buffer_at();
        bool ignore_next = false;
        while (c > 0 && (c != open_quote || ignore_next)) {
            ignore_next = c == '\\';
            data.push_back(c);
            advance();
            c = buffer_at();
        }

        // skip close ", ' or `
        if (c == open_quote) advance();
        else todof("error to find `%c` at the end of the string or the rune", open_quote);
        
        token.kind = open_quote == '`' ? TokenKind::Rune : TokenKind::String;
        token.raw = data;
    }
}

auto fmt::formatter<W::TokenKind>::format(W::TokenKind kind, format_context& ctx) const -> format_context::iterator {
    std::string_view value;
    switch (kind) {
        case W::TokenKind::Comment: value = "a comment"; break;
        case W::TokenKind::Ident: value = "an identifier"; break;
        case W::TokenKind::Integer: value = "an integer"; break;
        case W::TokenKind::Float: value = "an floating point number"; break;
        case W::TokenKind::String: value = "an string"; break;
        case W::TokenKind::Rune: value = "an character"; break;
        case W::TokenKind::Plus: value = "+"; break;
        case W::TokenKind::Minus: value = "-"; break;
        case W::TokenKind::Mul: value = "*"; break;
        case W::TokenKind::Div: value = "/"; break;
        case W::TokenKind::Mod: value = "%"; break;
        case W::TokenKind::BitAnd: value = "&"; break;
        case W::TokenKind::BitOr: value = "|"; break;
        case W::TokenKind::BitXor: value = "^"; break;
        case W::TokenKind::Inc: value = "++"; break;
        case W::TokenKind::Dec: value = "--"; break;
        case W::TokenKind::LogicalAnd: value = "&&"; break;
        case W::TokenKind::LogicalOr: value = "||"; break;
        case W::TokenKind::Not: value = "!"; break;
        case W::TokenKind::BitNot: value = "~"; break;
        case W::TokenKind::Question: value = "?"; break;
        case W::TokenKind::Comma: value = ","; break;
        case W::TokenKind::Semicolon: value = ";"; break;
        case W::TokenKind::Colon: value = ":"; break;
        case W::TokenKind::Hash: value = "#"; break;
        case W::TokenKind::At: value = "@"; break;
        case W::TokenKind::LeftShift: value = "<<"; break;
        case W::TokenKind::RightShift: value = ">>"; break;
        case W::TokenKind::UnsignedRightShift: value = ">>>"; break;
        case W::TokenKind::NotIn: value = "!in"; break;
        case W::TokenKind::NotIs: value = "!is"; break;
        case W::TokenKind::Assign: value = "="; break;
        case W::TokenKind::DeclAssign: value = ":="; break;
        case W::TokenKind::PlusAssign: value = "+="; break;
        case W::TokenKind::MinusAssign: value = "-="; break;
        case W::TokenKind::DivAssign: value = "/="; break;
        case W::TokenKind::MultAssign: value = "*="; break;
        case W::TokenKind::XorAssign: value = "^="; break;
        case W::TokenKind::ModAssign: value = "%="; break;
        case W::TokenKind::OrAssign: value = "|="; break;
        case W::TokenKind::AndAssign: value = "&="; break;
        case W::TokenKind::RightShiftAssign: value = "<<="; break;
        case W::TokenKind::LeftShiftAssign: value = ">>="; break;
        case W::TokenKind::UnsignedRightShiftAssign: value = ">>>="; break;
        case W::TokenKind::Lcbr: value = "{"; break;
        case W::TokenKind::Rcbr: value = "}"; break;
        case W::TokenKind::Lpar: value = "("; break;
        case W::TokenKind::Rpar: value = ")"; break;
        case W::TokenKind::Lsbr: value = "["; break;
        case W::TokenKind::Nilsbr: value = "#["; break;
        case W::TokenKind::Rsbr: value = "]"; break;
        case W::TokenKind::Eq: value = "=="; break;
        case W::TokenKind::Ne: value = "!="; break;
        case W::TokenKind::Gt: value = ">"; break;
        case W::TokenKind::Lt: value = "<"; break;
        case W::TokenKind::Ge: value = ">="; break;
        case W::TokenKind::Le: value = "<="; break;
        case W::TokenKind::Dot: value = "."; break;
        case W::TokenKind::Dotdot: value = ".."; break;
        case W::TokenKind::Ellipsis: value = "..."; break;
        case W::TokenKind::KeyAs: value = "as"; break;
        case W::TokenKind::KeyAsm: value = "asm"; break;
        case W::TokenKind::KeyAssert: value = "assert"; break;
        case W::TokenKind::KeyAtomic: value = "atomic"; break;
        case W::TokenKind::KeyBreak: value = "break"; break;
        case W::TokenKind::KeyConst: value = "const"; break;
        case W::TokenKind::KeyContinue: value = "continue"; break;
        case W::TokenKind::KeyDefer: value = "defer"; break;
        case W::TokenKind::KeyElse: value = "else"; break;
        case W::TokenKind::KeyEnum: value = "enum"; break;
        case W::TokenKind::KeyFalse: value = "false"; break;
        case W::TokenKind::KeyFor: value = "for"; break;
        case W::TokenKind::KeyFn: value = "fn"; break;
        case W::TokenKind::KeyGlobal: value = "global"; break;
        case W::TokenKind::KeyGoto: value = "goto"; break;
        case W::TokenKind::KeyIf: value = "if"; break;
        case W::TokenKind::KeyImport: value = "import"; break;
        case W::TokenKind::KeyIn: value = "in"; break;
        case W::TokenKind::KeyInterface: value = "interface"; break;
        case W::TokenKind::KeyIs: value = "is"; break;
        case W::TokenKind::KeyMatch: value = "match"; break;
        case W::TokenKind::KeyModule: value = "module"; break;
        case W::TokenKind::KeyMut: value = "mut"; break;
        case W::TokenKind::KeyNil: value = "nil"; break;
        case W::TokenKind::KeyNone: value = "none"; break;
        case W::TokenKind::KeyReturn: value = "return"; break;
        case W::TokenKind::KeySizeof: value = "sizeof"; break;
        case W::TokenKind::KeyLikely: value = "likely"; break;
        case W::TokenKind::KeyUnlikely: value = "unlikely"; break;
        case W::TokenKind::KeyOffsetof: value = "offsetof"; break;
        case W::TokenKind::KeyStruct: value = "struct"; break;
        case W::TokenKind::KeyTrue: value = "true"; break;
        case W::TokenKind::KeyType: value = "type"; break;
        case W::TokenKind::KeyTypeof: value = "typeof"; break;
        case W::TokenKind::KeyDump: value = "dump"; break;
        case W::TokenKind::KeyOr: value = "or"; break;
        case W::TokenKind::KeyUnion: value = "union"; break;
        case W::TokenKind::KeyPub: value = "pub"; break;
        case W::TokenKind::KeyStatic: value = "static"; break;
        case W::TokenKind::KeyVolatile: value = "volatile"; break;
        case W::TokenKind::KeyUnsafe: value = "unsafe"; break;
        case W::TokenKind::Eof: value = "eof"; break;

        default: value = "unknown"; break;
    }

    return formatter<std::string_view>::format(value, ctx);
}
