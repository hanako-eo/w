#ifndef W_PASS_HPP
#define W_PASS_HPP

#include <vector>

#include <frontend/ast/nodes.hpp>
#include <frontend/ast/expression_type.hpp>

namespace W::Passes {
    struct VisitorPass {
        void handle_statement(std::vector<Ast::StatementPtr>& node);
        void handle_statement(Ast::StatementPtr& node);
        void handle_expression(std::vector<Ast::ExpressionPtr>& node);
        void handle_expression(Ast::ExpressionPtr& node);

        void handle_children(Ast::ExpressionType<>& node);

        #define WLANG_AST(node_type, category) \
            void handle_children(Ast::node_type##category& node); \
            virtual void visit(Ast::node_type##category& node) = 0;
        #include <frontend/ast/node_list.hpp>
    };
}

#endif