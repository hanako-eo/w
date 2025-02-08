#include <iostream>

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <frontend/lexer.hpp>
#include <frontend/parser.hpp>
#include <frontend/token_stream.hpp>

int main(int argc, char **argv) {
    std::istringstream data(
        "mut"
    );
    W::Lexer lexer("test.w", data);
    W::TokenStream token_stream(lexer);

    return 0;
}
