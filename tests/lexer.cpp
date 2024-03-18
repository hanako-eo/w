#include <optional>
#include <utility>
#include <sstream>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <frontend/lexer.hpp>

TEST_CASE("lexing") {
    SECTION("primitives") {
        std::istringstream data("\"test\" 'it\\'s a test' \n0 `A` `\\n`");
        W::Lexer lexer("test.w", std::move(data));

        std::pair<W::TokenKind, std::optional<std::string_view>> expecteds[] = {
            {W::TokenKind::String, std::string_view("test")},
            {W::TokenKind::String, std::string_view("it's a test")},
            {W::TokenKind::Number, std::string_view("0")},
            {W::TokenKind::Rune, std::string_view("A")},
            {W::TokenKind::Rune, std::string_view("\n")},
        };
        for (auto expected : expecteds) {
            W::Token token = lexer.next();

            CHECK(expected.first == token.kind);
            CHECK(expected.second == token.data);
        }

        W::Token token = lexer.next();
        CHECK(token.kind == W::TokenKind::Eof);
    }
}
