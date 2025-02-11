#include <frontend/ast.hpp>

namespace W::Ast {
    #define WLANG_AST(category) NodeType category::get_type() const { \
        return NodeType::category; \
    }
    #include <frontend/node_list.hpp>
}

