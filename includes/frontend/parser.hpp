#ifndef W_PARSER_HPP
#define W_PARSER_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <frontend/ast.hpp>
#include <frontend/lexer.hpp>
#include <frontend/token_stream.hpp>

namespace W {
    class Parser {
    public:
        Parser(TokenStream& token_stream);
        ~Parser() = default;

        Ast::StatementPtr next();

    private:
        template<std::size_t N>
        const Token& expected(std::array<TokenKind, N> kind);
        const Token& expected(TokenKind kind);
        
        template<std::size_t N>
        bool start_by(std::array<TokenKind, N> kind);
        bool start_by(TokenKind kind);

        Ast::StatementPtr parse_func_declaration();
        Ast::StatementPtr parse_var_like_declaration();
        // Ast::StatementPtr parse_enum_declaration();
        // Ast::StatementPtr parse_interface_declaration();
        // Ast::StatementPtr parse_defer();
        // Ast::StatementPtr parse_struct_declaration();

        Ast::ExpressionPtr parse_expr(int precedence = 0);
        Ast::ExpressionPtr parse_binary(int precedence, Ast::ExpressionPtr lhs);
        Ast::ExpressionPtr parse_unary();
        Ast::ExpressionPtr parse_access(Ast::ExpressionPtr member);
        Ast::ExpressionPtr parse_primitive();
        Ast::ExpressionPtr parse_bool();
        Ast::ExpressionPtr parse_int();
        Ast::ExpressionPtr parse_float();
        Ast::ExpressionPtr parse_rune();
        Ast::ExpressionPtr parse_string();
        Ast::ExpressionPtr parse_ident();

        std::vector<Ast::ExpressionPtr> parse_expr_list(TokenKind termination_token, Location* termination_location);

        TokenStream& m_token_stream;
    };
}

#endif
