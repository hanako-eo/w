#include <frontend/ast/nodes.hpp>
#include <passes/pass.hpp>

namespace W::Ast {
    #define WLANG_AST(X, C) NodeType X##C::get_type() const { \
        return NodeType::X##C; \
    } \
    void X##C::visit(Passes::VisitorPass& visitor) { \
        visitor.visit(*this); \
    }
    #include <frontend/ast/node_list.hpp>
}

