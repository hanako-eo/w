#include <array>
#include <codecvt>
#include <cstddef>
#include <memory>

#include <errors.hpp>
#include <utils/utility.hpp>
#include <frontend/ast.hpp>
#include <frontend/lexer.hpp>
#include <frontend/parser.hpp>
#include <frontend/token_stream.hpp>

namespace W {
    static int get_token_precedence(TokenKind kind);

    Parser::Parser(TokenStream& token_stream):
        m_token_stream(token_stream)
    {}

    template<std::size_t N>
    const Token& Parser::expected(std::array<TokenKind, N> kinds) {
        const Token& token = m_token_stream.next();
        
        for (auto it = kinds.begin(); it < kinds.end(); it++) {
            if (token.kind == *it)
                return token;
        }

        throw ParserUnexpectedTokenError(token.location, token.kind);
    }

    const Token& Parser::expected(TokenKind kind) {
        const Token& token = m_token_stream.next();
        
        if (token.kind != kind)
            throw ParserExpectedTokenError(token.location, kind, token.kind);

        return token;
    }

    template<std::size_t N>
    bool Parser::start_by(std::array<TokenKind, N> kinds) {
        auto peeked_kind = m_token_stream.peek().kind;

        for (auto it = kinds.begin(); it < kinds.end(); it++) {
            if (peeked_kind == *it)
                return true;
        }
        
        return false;
    }

    bool Parser::start_by(TokenKind kind) {
        bool starting_by = m_token_stream.peek().kind == kind;

        if (starting_by) {
            m_token_stream.next();
        }

        return starting_by;
    }

    Ast::StatementPtr Parser::next() {
        bool is_pub = start_by(TokenKind::KeyPub);

        TokenKind kind = m_token_stream.peek().kind;
        Ast::StatementPtr stmt = nullptr;
        switch (kind) {
            case TokenKind::KeyFn: {
                stmt = parse_func_declaration();
                break;
            }
            case TokenKind::KeyConst:
            case TokenKind::KeyType:
            case TokenKind::KeyMut:
            case TokenKind::KeyStatic:
            case TokenKind::KeyVolatile: {
                stmt = parse_var_like_declaration();
                break;
            }
            case TokenKind::Ident: {
                const Token& next = m_token_stream.peek(1);

                if (next.kind == TokenKind::DeclAssign) {
                    stmt = parse_var_like_declaration();
                    break;
                }
            }
            default: {
                auto expr_stmt = std::make_unique<Ast::ExpressionStatement>();
                expr_stmt->expr = parse_expr();
                expr_stmt->location = expr_stmt->expr->location;

                stmt = std::move(expr_stmt);
            }
        }
        
        stmt->is_pub = is_pub;
        return stmt;
    }

    Ast::StatementPtr Parser::parse_func_declaration() {
        using FuncParam = Ast::DeclareFunctionStatement::Parameter;
        using VarMod = Ast::VariableModifiers;

        auto declare_func = std::make_unique<Ast::DeclareFunctionStatement>();
        Location start_location = expected(TokenKind::KeyFn).location;
        
        declare_func->name = expected(TokenKind::Ident).raw;
        
        // parse the function input
        expected(TokenKind::Lpar);
        const Token* token = &m_token_stream.peek();
        while (token->kind != TokenKind::Rpar && token->kind != TokenKind::Eof) {
            Location start_param_location = m_token_stream.peek().location;
            FuncParam param;
            
            if (start_by(TokenKind::KeyVolatile))
            param.modifiers |= VarMod::Volatile;
            if (start_by(TokenKind::KeyMut))
            param.modifiers |= VarMod::Mutable;
            
            param.name = expected(TokenKind::Ident).raw;
            param.type = parse_expr();
            param.location = Location::merge(start_location, param.type->location);
            
            declare_func->parameters.push_back(std::move(param));

            if (!start_by(TokenKind::Comma))
                break;

            token = &m_token_stream.peek();
        }
        expected(TokenKind::Rpar);

        // parse the function body and the return type if it exists
        if (!start_by(TokenKind::Lcbr)) {
            declare_func->return_type = parse_expr();
            expected(TokenKind::Lcbr);
        }

        token = &m_token_stream.peek();
        while (token->kind != TokenKind::Rcbr && token->kind != TokenKind::Eof) {
            declare_func->body.push_back(next());
            token = &m_token_stream.peek();
        }
        Location end_location = expected(TokenKind::Rcbr).location;

        declare_func->location = Location::merge(start_location, end_location);

        return declare_func;
    }

    Ast::StatementPtr Parser::parse_var_like_declaration() {
        using VarMod = Ast::VariableModifiers;

        const Token* modifier_token = &m_token_stream.peek();
        Location start_location = modifier_token->location;

        VarMod modifiers;
        switch (modifier_token->kind) {
            case TokenKind::KeyConst: modifiers = VarMod::Const; break;
            case TokenKind::KeyType: modifiers = VarMod::Type; break;
            case TokenKind::KeyStatic: modifiers = VarMod::Static; break;
            default: modifiers = VarMod::None; break;
        }
        
        if (modifiers) {
            m_token_stream.next();
            modifier_token = &m_token_stream.peek();
        }
        
        if(modifier_token->kind == TokenKind::KeyVolatile) {
            modifiers |= VarMod::Volatile;
            m_token_stream.next();
            modifier_token = &m_token_stream.peek();
        }

        if(modifier_token->kind == TokenKind::KeyMut) {
            if ((modifiers & VarMod::Const) != 0)
                throw ParserUnexpectedConstMutabilityError(modifier_token->location);
            else if ((modifiers & VarMod::Type) != 0)
                throw ParserUnexpectedTypeMutabilityError(modifier_token->location);
                
            modifiers |= VarMod::Mutable;
            m_token_stream.next();
            modifier_token = &m_token_stream.peek();
        }

        const Token& name_token = expected(TokenKind::Ident);

        expected(TokenKind::DeclAssign);

        Ast::ExpressionPtr value = parse_expr();

        auto declare_var = std::make_unique<Ast::DeclareVariableStatement>();
        declare_var->modifiers = modifiers;
        declare_var->name = std::move(name_token.raw);
        declare_var->location = Location::merge(start_location, value->location);
        declare_var->value = std::move(value);

        return declare_var;
    }

    // Ast::StatementPtr Parser::parse_enum_declaration();
    // Ast::StatementPtr Parser::parse_interface_declaration();
    // Ast::StatementPtr Parser::parse_struct_declaration();
    // Ast::StatementPtr Parser::parse_defer();

    Ast::ExpressionPtr Parser::parse_expr(int precedence) {
        return parse_binary(precedence, parse_unary());
    }

    Ast::ExpressionPtr Parser::parse_binary(int precedence, Ast::ExpressionPtr lhs) {
        const Token& curr_op = m_token_stream.peek();

        int op_precedence = get_token_precedence(curr_op.kind);
        if (op_precedence < precedence)
            return lhs;

        m_token_stream.next();

        Ast::ExpressionPtr rhs = parse_unary();

        const Token& next_op = m_token_stream.peek();

        int next_op_precedence = get_token_precedence(next_op.kind);
        if (op_precedence < next_op_precedence)
            rhs = parse_binary(precedence, std::move(rhs));

        Ast::BinaryOp bin_op;
        switch (curr_op.kind) {
            case TokenKind::BitAnd: bin_op = Ast::BinaryOp::BitwiseAnd; break;
            case TokenKind::BitOr: bin_op = Ast::BinaryOp::BitwiseOr; break;
            case TokenKind::BitXor: bin_op = Ast::BinaryOp::BitwiseXor; break;
            case TokenKind::Div: bin_op = Ast::BinaryOp::Divide; break;
            case TokenKind::Eq: bin_op = Ast::BinaryOp::CompEq; break;
            case TokenKind::Lt: bin_op = Ast::BinaryOp::CompLt; break;
            case TokenKind::Le: bin_op = Ast::BinaryOp::CompLe; break;
            case TokenKind::LogicalAnd: bin_op = Ast::BinaryOp::LogicalAnd; break;
            case TokenKind::LogicalOr: bin_op = Ast::BinaryOp::LogicalOr; break;
            case TokenKind::Gt: bin_op = Ast::BinaryOp::CompGt; break;
            case TokenKind::Ge: bin_op = Ast::BinaryOp::CompGe; break;
            case TokenKind::Mod: bin_op = Ast::BinaryOp::Modulo; break;
            case TokenKind::Minus: bin_op = Ast::BinaryOp::Subtract; break;
            case TokenKind::Mul: bin_op = Ast::BinaryOp::Multiply; break;
            case TokenKind::Ne: bin_op = Ast::BinaryOp::CompNe; break;
            case TokenKind::Plus: bin_op = Ast::BinaryOp::Add; break;
            case TokenKind::LeftShift: bin_op = Ast::BinaryOp::ShiftLeft; break;
            case TokenKind::RightShift: bin_op = Ast::BinaryOp::ShiftRight; break;
            case TokenKind::UnsignedRightShift: bin_op = Ast::BinaryOp::UnsignedShiftRight; break;

            // is not a symbol of the current expression (it will be considered as a new statement or expression)
            default: return lhs;
        }

        auto binary_expr = std::make_unique<Ast::BinaryExpression>();
        binary_expr->location = Location::merge(lhs->location, rhs->location);
        binary_expr->left = std::move(lhs);
        binary_expr->right = std::move(rhs);
        binary_expr->op = bin_op;

        return parse_binary(precedence, std::move(binary_expr));
    }

    Ast::ExpressionPtr Parser::parse_unary() {
        const Token& token = m_token_stream.peek();
        Ast::ExpressionPtr final_expr;
        Ast::ExpressionPtr* base;

        if (token.kind == TokenKind::Lsbr) {
            expected(TokenKind::Lsbr);
            if (start_by(TokenKind::Rsbr)) {
                auto type_expr = std::make_unique<Ast::SliceTypeExpression>();
                
                base = &type_expr->inner_type;
                final_expr = std::move(type_expr);
            } else {
                auto type_expr = std::make_unique<Ast::ArrayTypeExpression>();

                type_expr->lenght = parse_expr();
                expected(TokenKind::Rsbr);

                base = &type_expr->inner_type;
                final_expr = std::move(type_expr);
            }
        } else {
            Ast::UnaryOp op;
            switch (token.kind) {
                case TokenKind::Plus: op = Ast::UnaryOp::Plus; break;
                case TokenKind::Minus: op = Ast::UnaryOp::Minus; break;
                case TokenKind::Mul: op = Ast::UnaryOp::Deref; break;
                case TokenKind::BitAnd: op = Ast::UnaryOp::Ref; break;
                case TokenKind::Not: op = Ast::UnaryOp::Not; break;
                case TokenKind::Question: op = Ast::UnaryOp::Option; break;
                
                case TokenKind::Inc: throw ParserUnsupportedPrefixIncError(token.location);
                case TokenKind::Dec: throw ParserUnsupportedPrefixDecError(token.location);
    
                default:
                    return parse_access(parse_primitive());
            }
            m_token_stream.next();

            auto unary_expr = std::make_unique<Ast::UnaryExpression>();
            unary_expr->op = op;

            base = &unary_expr->expr;
            final_expr = std::move(unary_expr);
        }
        
        *base = parse_unary();
        final_expr->location = Location::merge(token.location, (*base)->location);

        return final_expr;
    }

    Ast::ExpressionPtr Parser::parse_access(Ast::ExpressionPtr member) {
        const Token& token = m_token_stream.peek();
        if (token.kind == TokenKind::Dot) {
            expected(TokenKind::Dot);

            if (member->get_type() == Ast::NodeType::AccessIdentifierExpression) {
                auto access_identifier = static_cast<Ast::AccessIdentifierExpression*>(member.get());
                access_identifier->members.push_back(parse_primitive());

                return parse_access(std::move(member));
            } else {
                auto access_identifier = std::make_unique<Ast::AccessIdentifierExpression>();
                access_identifier->members.push_back(std::move(member));

                return parse_access(std::move(access_identifier));
            }
        }

        if (token.kind == TokenKind::Lpar || token.kind == TokenKind::Lsbr) {
            expected(std::array { TokenKind::Lpar, TokenKind::Lsbr });

            Location end_location;
            std::vector<Ast::ExpressionPtr> params = parse_expr_list(
                token.kind == TokenKind::Lpar ? TokenKind::Rpar : TokenKind::Rsbr,
                &end_location
            );

            auto call_expr = std::make_unique<Ast::CallExpression>();
            call_expr->callee = std::move(member);
            call_expr->params = std::move(params);

            return parse_access(std::move(call_expr));
        }

        return member;
    }

    Ast::ExpressionPtr Parser::parse_primitive() {
        switch (m_token_stream.peek().kind) {
            case TokenKind::KeyTrue:
            case TokenKind::KeyFalse: {
                return parse_bool();
            }
            case TokenKind::Integer: {
                return parse_int();
            }
            case TokenKind::Float: {
                return parse_float();
            }
            case TokenKind::Rune: {
                return parse_rune();
            }
            case TokenKind::String: {
                return parse_string();
            }
            case TokenKind::Ident: {
                return parse_ident();
            }
            case TokenKind::Dot: {
                return parse_enum_variant();
            }
            case TokenKind::Lpar: {
                Location start_location = expected(TokenKind::Lpar).location;
                auto parent_expr = std::make_unique<Ast::ParentExpression>();
                parent_expr->expr = parse_expr();
                parent_expr->location = Location::merge(start_location, expected(TokenKind::Rpar).location);
                return parent_expr;
            }
            default: {
                utils::unreachable();
            }
        }
    }

    Ast::ExpressionPtr Parser::parse_bool() {
        const Token& token = expected(std::array { TokenKind::KeyTrue, TokenKind::KeyFalse });
        auto lit = std::make_unique<Ast::BoolLiteral>();
        lit->value = token.kind == TokenKind::KeyTrue ? true : false;
        lit->location = token.location;

        return lit;
    }

    Ast::ExpressionPtr Parser::parse_int() {
        const Token& token = expected(TokenKind::Integer);
        auto lit = std::make_unique<Ast::IntLiteral>();
        lit->value = std::stol(token.raw);
        lit->location = token.location;

        return lit;
    }

    Ast::ExpressionPtr Parser::parse_float() {
        const Token& token = expected(TokenKind::Float);
        auto lit = std::make_unique<Ast::FloatLiteral>();
        lit->value = std::stod(token.raw);
        lit->raw = std::move(token.raw);
        lit->location = token.location;

        return lit;
    }
    
    Ast::ExpressionPtr Parser::parse_rune() {
        static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8_codec;
        
        const Token& token = expected(TokenKind::Rune);
        try {
            auto rune = utf8_codec.from_bytes(token.raw);
            if (rune.empty())
                throw ParserEmptyRuneError(token.location);
            
            if (rune.length() > 1)
                throw ParserRuneIsNotAStringError(token.location, std::move(token.raw));
            
            auto lit = std::make_unique<Ast::RuneLiteral>();
            lit->value = rune[0];
            lit->location = token.location;
            
            return lit;
        } catch (std::range_error const&) {
            throw ParserIllFormedRuneError(token.location, std::move(token.raw));
        }
    }

    Ast::ExpressionPtr Parser::parse_string() {
        const Token& token = expected(TokenKind::String);
        auto lit = std::make_unique<Ast::StringLiteral>();
        lit->value = std::move(token.raw);
        lit->location = token.location;
        
        return lit;
    }
    
    Ast::ExpressionPtr Parser::parse_ident() {
        const Token& token = expected(TokenKind::Ident);
        auto lit = std::make_unique<Ast::IdentExpression>();
        lit->value = std::move(token.raw);
        lit->location = token.location;

        return lit;
    }
    
    Ast::ExpressionPtr Parser::parse_enum_variant() {
        Location dot = expected(TokenKind::Dot).location;
        const Token& token = expected(TokenKind::Ident);
        auto lit = std::make_unique<Ast::EnumVariantLiteral>();
        lit->value = std::move(token.raw);
        lit->location = Location::merge(dot, token.location);

        return lit;
    }

    std::vector<Ast::ExpressionPtr> Parser::parse_expr_list(TokenKind termination_token, Location* termination_location) {
        std::vector<Ast::ExpressionPtr> parameters;

        while (m_token_stream.peek().kind != termination_token) {
            parameters.push_back(parse_expr());
                
            if (m_token_stream.peek().kind != TokenKind::Comma)
                break;
            expected(TokenKind::Comma);
        }

        const Token& token = expected(termination_token);
        if (termination_location == nullptr)
            *termination_location = token.location;

        return parameters;
    }

    static int get_token_precedence(TokenKind kind) {
        switch (kind) {
            case TokenKind::LogicalOr: return 2;
            case TokenKind::LogicalAnd: return 4;
            case TokenKind::BitOr: return 6;
            case TokenKind::BitXor: return 8;
            case TokenKind::BitAnd: return 10;
            case TokenKind::Lt: return 20;
            case TokenKind::Le: return 20;
            case TokenKind::Gt: return 20;
            case TokenKind::Ge: return 20;
            case TokenKind::Ne: return 20;
            case TokenKind::Eq: return 20;
            case TokenKind::LeftShift: return 30;
            case TokenKind::RightShift: return 30;
            case TokenKind::Plus: return 40;
            case TokenKind::Minus: return 40;
            case TokenKind::Mul: return 50;
            case TokenKind::Div: return 50;
            case TokenKind::Mod: return 50;

            default: return -1;
        }
    }
}
