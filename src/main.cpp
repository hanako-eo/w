#include <iostream>

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <errors.hpp>
#include <frontend/lexer.hpp>
#include <frontend/parser.hpp>
#include <frontend/token_stream.hpp>

int main(int argc, char **argv) {
    std::istringstream data(
        "mut a := 0"
    );
    W::Lexer lexer("test.w", data);
    W::TokenStream token_stream(lexer);
    W::Parser parser(token_stream);

    try {
        auto stmt = parser.next();
        assert(stmt->get_type() == W::Ast::NodeType::DeclareVariableStatement);
    } catch (W::Exception& e) {
        fmt::print("{} error: {}\n", e.get_location(), e.what());
    }

    return 0;
}
