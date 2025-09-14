
#ifndef W_EXPRESSION_TYPE_HPP
#define W_EXPRESSION_TYPE_HPP

#include <memory>
#include <variant>

namespace W::Ast {
    struct Expression;
    using ExpressionPtr = std::unique_ptr<Expression>;

    struct Type;
    using TypePtr = std::unique_ptr<Type>;

    template<bool Optional = false>
    struct ExpressionType {
        ExpressionType() requires (Optional) = default;
        ExpressionType(TypePtr type);
        ExpressionType(ExpressionPtr expr);
        ExpressionType(const ExpressionType&) = delete;
        ExpressionType(ExpressionType&&) noexcept = default;
        ~ExpressionType() = default;

        ExpressionType& operator=(const ExpressionType&) = delete;
        ExpressionType& operator=(ExpressionType&&) noexcept = default;

        bool is_expression() const;
        ExpressionPtr& get_expression();

        bool is_type() const;
        TypePtr& get_type();
        
        bool has_value() const;

    private:
        // maybe use NodePtr instead ? (dynamic_pointer_cast)
        std::conditional_t<
            Optional, 
            std::variant<std::monostate, ExpressionPtr, TypePtr>, 
            std::variant<ExpressionPtr, TypePtr>
        > m_value;
    };
}

#include <frontend/ast/expression_type.inl>

#endif
