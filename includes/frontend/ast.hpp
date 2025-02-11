#ifndef W_AST_HPP
#define W_AST_HPP

#include <memory>
#include <string>
#include <vector>

#include <location.hpp>
#include <frontend/ast_enums.hpp>
#include <utils/types.hpp>

namespace W::Ast {
    struct Node {
        virtual ~Node() = default;

        virtual NodeType get_type() const = 0;
        
        Location location;
    };
    using NodePtr = std::unique_ptr<Node>;

    // EXPRESSION

    struct Expression : Node {
        virtual ~Expression() = default;
    };
    using ExpressionPtr = std::unique_ptr<Expression>;

    struct AccessIdentifierExpression : Expression {
        NodeType get_type() const override;

        std::vector<ExpressionPtr> members;
    };

    struct CallExpression : Expression {
        NodeType get_type() const override;

        ExpressionPtr callee;
        std::vector<ExpressionPtr> params;
    };

    struct BinaryExpression : Expression {
        NodeType get_type() const override;

        BinaryOp op;
        ExpressionPtr left;
        ExpressionPtr right;
    };

    struct UnaryExpression : Expression {
        NodeType get_type() const override;

        UnaryOp op;
        ExpressionPtr expr;
    };

    struct IdentExpression : Expression {
        NodeType get_type() const override;

        std::string value;
    };

    struct ParentExpression : Expression {
        NodeType get_type() const override;

        ExpressionPtr expr;
    };

    // LITERALS

    struct BoolLiteral : Expression {
        NodeType get_type() const override;

        bool value;
    };

    struct IntLiteral : Expression {
        NodeType get_type() const override;

        int64_t value;
    };

    struct FloatLiteral : Expression {
        NodeType get_type() const override;

        float64_t value;
        // needed to avoid issue due to the precission of a float (during the
        // translation into C)
        std::string raw;
    };

    struct RuneLiteral : Expression {
        NodeType get_type() const override;

        char32_t value;
    };

    struct StringLiteral : Expression {
        NodeType get_type() const override;

        std::string value;
    };

    // STATEMENTS
    
    struct Statement : Node {
        virtual ~Statement() = default;
    };
    using StatementPtr = std::unique_ptr<Statement>;

    struct ExpressionStatement : Statement {
        NodeType get_type() const override;

        ExpressionPtr expr;
    };

    struct DeclareVariableStatement : Statement {
        NodeType get_type() const override;

        bool is_mutable;
        std::string name;
        ExpressionPtr value;
    };
}

#endif