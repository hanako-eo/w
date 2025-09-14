#ifndef W_AST_HPP
#define W_AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <optional>

#include <location.hpp>
#include <frontend/ast/node_enums.hpp>
#include <frontend/ast/expression_type.hpp>
#include <utils/types.hpp>

namespace W::Passes {
    struct VisitorPass;
}

namespace W::Ast {
    enum struct VariableModifiers {
        None = 0,
        Mutable = 1 << 0,
        Const = 1 << 1,
        Type = 1 << 2,
        Static = 1 << 3,
        Volatile = 1 << 4,
    };

    struct Node {
        Node() = default;
        Node(const Node&) = delete;
        Node(Node&&) noexcept = default;
        virtual ~Node() = default;

        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) noexcept = default;

        virtual NodeType get_type() const = 0;
        virtual void visit(Passes::VisitorPass& visitor) = 0;

        Location location;
    };
    using NodePtr = std::unique_ptr<Node>;

    // TYPES

    struct Type {
        ~Type() = default;

        // virtual size_t get_id() const = 0;
    };
    using TypePtr = std::unique_ptr<Type>;

    // EXPRESSIONS

    struct Expression : Node {
        ~Expression() = default;
    };
    using ExpressionPtr = std::unique_ptr<Expression>;

    struct AccessIdentifierExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        std::vector<ExpressionPtr> members;
    };

    struct CallExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        ExpressionPtr callee;
        std::vector<ExpressionPtr> params;
    };

    struct BinaryExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        BinaryOp op;
        ExpressionPtr left;
        ExpressionPtr right;
    };

    struct UnaryExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        UnaryOp op;
        ExpressionPtr expr;
    };
    
    struct IdentExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;
        
        std::string value;
    };
    
    struct ParentExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;
        
        ExpressionPtr expr;
    };

    // TYPE EXPRESSIONS

    struct ArrayTypeExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        ExpressionPtr lenght;
        ExpressionPtr inner_type;
    };

    struct SliceTypeExpression : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        ExpressionPtr inner_type;
    };

    // LITERALS

    struct BoolLiteral : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        bool value;
    };

    struct IntLiteral : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        int64_t value;
    };

    struct FloatLiteral : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        float64_t value;
        // needed to avoid issue due to the precission of a float (during the
        // translation into C)
        std::string raw;
    };

    struct EnumVariantLiteral : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        std::string value;
    };

    struct RuneLiteral : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        char32_t value;
    };

    struct StringLiteral : Expression {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        std::string value;
    };

    // STATEMENTS
    
    struct Statement : Node {
        ~Statement() = default;

        bool is_pub;
    };
    using StatementPtr = std::unique_ptr<Statement>;

    struct ExpressionStatement : Statement {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        ExpressionPtr expr;
    };

    struct DeclareFunctionStatement : Statement {
        struct Parameter {
            Location location;
            VariableModifiers modifiers;
            std::string name;
            ExpressionType<> type;
        };
        
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;

        std::string name;
        ExpressionType<true> return_type;
        std::vector<Parameter> parameters;
        std::vector<StatementPtr> body;
    };

    struct DeclareVariableStatement : Statement {
        NodeType get_type() const override;
        void visit(Passes::VisitorPass& visitor) override;
        
        VariableModifiers modifiers;
        std::string name;
        ExpressionPtr value;
    };

    inline constexpr VariableModifiers operator&(VariableModifiers lhs, VariableModifiers rhs) {
        return static_cast<VariableModifiers>(
            static_cast<std::underlying_type_t<VariableModifiers>>(lhs) &
            static_cast<std::underlying_type_t<VariableModifiers>>(rhs)
        );
    }

    inline constexpr VariableModifiers operator|(VariableModifiers lhs, VariableModifiers rhs) {
        return static_cast<VariableModifiers>(
            static_cast<std::underlying_type_t<VariableModifiers>>(lhs) |
            static_cast<std::underlying_type_t<VariableModifiers>>(rhs)
        );
    }

    inline constexpr VariableModifiers& operator|=(VariableModifiers& lhs, VariableModifiers rhs) {
        return lhs = static_cast<VariableModifiers>(lhs | rhs);
    }
}

#endif
