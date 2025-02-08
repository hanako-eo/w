#include <variant>
#include <utility>
#include <sstream>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <frontend/lexer.hpp>
#include <utils/types.hpp>

TEST_CASE("lexing") {
    SECTION("primitives") {
        std::istringstream data("\"test\" 'it\\'s a test' \n0 /* test */ 0.5 `A` `\\n` // test\n /* /* test */ nesting */");
        W::Lexer lexer("test.w", data);

        std::pair<W::TokenKind, std::string> expecteds[] = {
            {W::TokenKind::String, "test"},
            {W::TokenKind::String, "it\\'s a test"},
            {W::TokenKind::Integer, "0"},
            {W::TokenKind::Float, "0.5"},
            {W::TokenKind::Rune, "A"},
            {W::TokenKind::Rune, "\\n"},
        };
        for (auto expected : expecteds) {
            W::Token token = lexer.next();

            CHECK(expected.first == token.kind);
            CHECK(expected.second == token.raw);
        }

        W::Token token = lexer.next();
        CHECK(token.kind == W::TokenKind::Eof);
    }

    SECTION("keywords") {
        std::istringstream data(
            "as asm assert atomic break const continue defer "
            "else enum false for fn global goto if import in interface is match module "
            "mut nil none return sizeof likely like unlikely offsetof struct true "
            "typeof type or union pub static volatile unsafe test1"
        );
        W::Lexer lexer("test.w", data);

        W::TokenKind expecteds[] = {
            W::TokenKind::KeyAs,
            W::TokenKind::KeyAsm,
            W::TokenKind::KeyAssert,
            W::TokenKind::KeyAtomic,
            W::TokenKind::KeyBreak,
            W::TokenKind::KeyConst,
            W::TokenKind::KeyContinue,
            W::TokenKind::KeyDefer,
            W::TokenKind::KeyElse,
            W::TokenKind::KeyEnum,
            W::TokenKind::KeyFalse,
            W::TokenKind::KeyFor,
            W::TokenKind::KeyFn,
            W::TokenKind::KeyGlobal,
            W::TokenKind::KeyGoto,
            W::TokenKind::KeyIf,
            W::TokenKind::KeyImport,
            W::TokenKind::KeyIn,
            W::TokenKind::KeyInterface,
            W::TokenKind::KeyIs,
            W::TokenKind::KeyMatch,
            W::TokenKind::KeyModule,
            W::TokenKind::KeyMut,
            W::TokenKind::KeyNil,
            W::TokenKind::KeyNone,
            W::TokenKind::KeyReturn,
            W::TokenKind::KeySizeof,
            W::TokenKind::KeyLikely,
            W::TokenKind::KeyLike,
            W::TokenKind::KeyUnlikely,
            W::TokenKind::KeyOffsetof,
            W::TokenKind::KeyStruct,
            W::TokenKind::KeyTrue,
            W::TokenKind::KeyTypeof,
            W::TokenKind::KeyType,
            W::TokenKind::KeyOr,
            W::TokenKind::KeyUnion,
            W::TokenKind::KeyPub,
            W::TokenKind::KeyStatic,
            W::TokenKind::KeyVolatile,
            W::TokenKind::KeyUnsafe,
        };
        for (auto expected : expecteds) {
            W::Token token = lexer.next();

            CHECK(expected == token.kind);
        }

        W::Token token1 = lexer.next();
        CHECK(W::TokenKind::Ident == token1.kind);
        CHECK("test1" == token1.raw);

        W::Token token = lexer.next();
        CHECK(W::TokenKind::Eof == token.kind);
    }

    SECTION("operators") {
        std::istringstream data(
            ">>>= >>> <<= << >>= >> >= > <= < == != := += -= /= *= ^= %= |= &= = "
            "!in !is ! ++ + -- - * / % ^ ~ ? , ; : && & || | @ { } ( ) #[ [ "
            "] # ... .. ."
        );
        W::Lexer lexer("test.w", data);

        W::TokenKind expecteds[] = {
            W::TokenKind::UnsignedRightShiftAssign,
            W::TokenKind::UnsignedRightShift,
            W::TokenKind::RightShiftAssign,
            W::TokenKind::LeftShift,
            W::TokenKind::LeftShiftAssign,
            W::TokenKind::RightShift,
            W::TokenKind::Ge,
            W::TokenKind::Gt,
            W::TokenKind::Le,
            W::TokenKind::Lt,
            W::TokenKind::Eq,
            W::TokenKind::Ne,
            W::TokenKind::DeclAssign,
            W::TokenKind::PlusAssign,
            W::TokenKind::MinusAssign,
            W::TokenKind::DivAssign,
            W::TokenKind::MultAssign,
            W::TokenKind::XorAssign,
            W::TokenKind::ModAssign,
            W::TokenKind::OrAssign,
            W::TokenKind::AndAssign,
            W::TokenKind::Assign,
            W::TokenKind::NotIn,
            W::TokenKind::NotIs,
            W::TokenKind::Not,
            W::TokenKind::Inc,
            W::TokenKind::Plus,
            W::TokenKind::Dec,
            W::TokenKind::Minus,
            W::TokenKind::Mul,
            W::TokenKind::Div,
            W::TokenKind::Mod,
            W::TokenKind::Xor,
            W::TokenKind::BitNot,
            W::TokenKind::Question,
            W::TokenKind::Comma,
            W::TokenKind::Semicolon,
            W::TokenKind::Colon,
            W::TokenKind::And,
            W::TokenKind::Amp,
            W::TokenKind::LogicalOr,
            W::TokenKind::Pipe,
            W::TokenKind::At,
            W::TokenKind::Lcbr,
            W::TokenKind::Rcbr,
            W::TokenKind::Lpar,
            W::TokenKind::Rpar,
            W::TokenKind::Nilsbr,
            W::TokenKind::Lsbr,
            W::TokenKind::Rsbr,
            W::TokenKind::Hash,
            W::TokenKind::Ellipsis,
            W::TokenKind::Dotdot,
            W::TokenKind::Dot,
        };
        for (auto expected : expecteds) {
            W::Token token = lexer.next();

            CHECK(expected == token.kind);
        }

        W::Token token = lexer.next();
        CHECK(W::TokenKind::Eof == token.kind);
    }
}
