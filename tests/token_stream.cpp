#include <variant>
#include <utility>
#include <sstream>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <frontend/lexer.hpp>
#include <frontend/token_stream.hpp>
#include <utils/types.hpp>

TEST_CASE("token_stream") {
    SECTION("normal iteration") {
        std::istringstream data("a := 69");
        W::Lexer lexer("test.w", data);
        W::TokenStream token_stream(lexer);

        std::pair<W::TokenKind, std::string> expecteds[] = {
            {W::TokenKind::Ident, "a"},
            {W::TokenKind::DeclAssign, ""},
            {W::TokenKind::Integer, "69"},
        };
        
        for (auto expected : expecteds) {
            W::Token token = token_stream.next();

            CHECK(expected.first == token.kind);
            CHECK(expected.second == token.raw);
        }

        W::Token token = token_stream.next();
        CHECK(token.kind == W::TokenKind::Eof);
    }
    SECTION("uncommit") {
        std::istringstream data("a := 69");
        W::Lexer lexer("test.w", data);
        W::TokenStream token_stream(lexer);

        std::pair<W::TokenKind, std::string> expecteds[] = {
            {W::TokenKind::Ident, "a"},
            {W::TokenKind::DeclAssign, ""},
            {W::TokenKind::Integer, "69"},
        };
        
        for (auto expected : expecteds) {
            W::Token token = token_stream.next();

            CHECK(expected.first == token.kind);
            CHECK(expected.second == token.raw);
        }

        // reset the iteration
        token_stream.uncommit();
        
        for (auto expected : expecteds) {
            W::Token token = token_stream.next();

            CHECK(expected.first == token.kind);
            CHECK(expected.second == token.raw);
        }

        W::Token token = token_stream.next();
        CHECK(token.kind == W::TokenKind::Eof);
    }
    SECTION("commit") {
        std::istringstream data("a := 69");
        W::Lexer lexer("test.w", data);
        W::TokenStream token_stream(lexer);

        std::pair<W::TokenKind, std::string> expecteds[] = {
            {W::TokenKind::Ident, std::string("a")},
            {W::TokenKind::DeclAssign, ""},
        };
        
        for (auto expected : expecteds) {
            W::Token token = token_stream.next();

            CHECK(expected.first == token.kind);
            CHECK(expected.second == token.raw);
        }
        token_stream.commit();

        W::Token token1 = token_stream.next();
        CHECK(W::TokenKind::Integer == token1.kind);
        CHECK("69" == token1.raw);

        token_stream.uncommit();

        W::Token token2 = token_stream.next();
        CHECK(W::TokenKind::Integer == token2.kind);
        CHECK("69" == token2.raw);
    }
}
