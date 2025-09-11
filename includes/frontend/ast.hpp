#ifndef W_AST_HPP
#define W_AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <optional>

#include <location.hpp>
#include <frontend/ast_enums.hpp>
#include <utils/types.hpp>

namespace W::Ast {
    enum VariableModifiers {
        None = 0,
        Mutable = 1 << 0,
        Const = 1 << 1,
        Type = 1 << 2,
        Static = 1 << 3,
        Volatile = 1 << 4,
    };

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

        bool is_pub;
    };
    using StatementPtr = std::unique_ptr<Statement>;

    struct ExpressionStatement : Statement {
        NodeType get_type() const override;

        ExpressionPtr expr;
    };

    struct DeclareFunctionStatement : Statement {
		struct Parameter {
            VariableModifiers modifiers;
			std::string name;
			ExpressionPtr type;
			// ExpressionValue<ExpressionType> type;
			Location location;
		};
        
        NodeType get_type() const override;
        
        std::string name;
        std::optional<ExpressionPtr> return_type;
        // ExpressionValue<ExpressionType> return_type;
        std::vector<Parameter> parameters;
		std::vector<StatementPtr> body;
    };

    struct DeclareVariableStatement : Statement {
        
        NodeType get_type() const override;
        
        VariableModifiers modifiers;
        std::string name;
        ExpressionPtr value;
    };

    inline constexpr VariableModifiers& operator|=(VariableModifiers& lhs, VariableModifiers rhs) {
        return lhs = static_cast<VariableModifiers>(lhs | rhs);
    }
}

#endif