#include <utils/utility.hpp>

namespace W::Ast {
    template<bool O>
    ExpressionType<O>::ExpressionType(TypePtr value) : m_value(std::move(value)) {}
    
    template<bool O>
    ExpressionType<O>::ExpressionType(ExpressionPtr value) : m_value(std::move(value)) {}
    
    template<bool O>
    bool ExpressionType<O>::is_expression() const {
        return std::holds_alternative<ExpressionPtr>(m_value);
    }
    
    template<bool O>
    ExpressionPtr& ExpressionType<O>::get_expression() {
        if (!is_expression())
            utils::panic("expected expression");
        
        return std::get<ExpressionPtr>(m_value);
    }
    
    template<bool O>
    bool ExpressionType<O>::is_type() const {
        return std::holds_alternative<TypePtr>(m_value);
    }
    
    template<bool O>
    TypePtr& ExpressionType<O>::get_type() {
        if (!is_type())
            utils::panic("expected type");
        
        return std::get<TypePtr>(m_value);
    }
    
    template<bool O>
    bool ExpressionType<O>::has_value() const {
        return O || std::holds_alternative<std::monostate>(m_value);
    }
}
