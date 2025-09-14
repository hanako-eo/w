#include <passes/pass.hpp>

namespace W::Passes {
    void VisitorPass::handle_statement(std::vector<Ast::StatementPtr>& stmt_list) {}
    void VisitorPass::handle_statement(Ast::StatementPtr& stmt) {}
    void VisitorPass::handle_expression(std::vector<Ast::ExpressionPtr>& expr_list) {}
    void VisitorPass::handle_expression(Ast::ExpressionPtr& expr) {}

    void VisitorPass::handle_children(Ast::ExpressionType<>& expr) {}

    void VisitorPass::handle_children(Ast::AccessIdentifierExpression& expr) {}
    void VisitorPass::handle_children(Ast::CallExpression& expr) {}
    void VisitorPass::handle_children(Ast::BinaryExpression& expr) {}
    void VisitorPass::handle_children(Ast::UnaryExpression& expr) {}
    void VisitorPass::handle_children(Ast::IdentExpression& expr) {}
    void VisitorPass::handle_children(Ast::ParentExpression& expr) {}
    void VisitorPass::handle_children(Ast::ArrayTypeExpression& expr) {}
    void VisitorPass::handle_children(Ast::SliceTypeExpression& expr) {}

    void VisitorPass::handle_children(Ast::BoolLiteral& lit) {}
    void VisitorPass::handle_children(Ast::IntLiteral& lit) {}
    void VisitorPass::handle_children(Ast::FloatLiteral& lit) {}
    void VisitorPass::handle_children(Ast::EnumVariantLiteral& lit) {}
    void VisitorPass::handle_children(Ast::RuneLiteral& lit) {}
    void VisitorPass::handle_children(Ast::StringLiteral& lit) {}

    void VisitorPass::handle_children(Ast::ExpressionStatement& stmt) {}
    void VisitorPass::handle_children(Ast::DeclareFunctionStatement& stmt) {}
    void VisitorPass::handle_children(Ast::DeclareVariableStatement& stmt) {}
}
