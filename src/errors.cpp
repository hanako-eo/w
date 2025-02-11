#include <errors.hpp>

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
        case W::TokenKind::KeyLike: value = "like"; break;
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

        default: value = "unknown"; break;
    }

    return formatter<std::string_view>::format(value, ctx);
}

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