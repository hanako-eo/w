#ifndef W_AST_ENUMS_HPP
#define W_AST_ENUMS_HPP

namespace W::Ast {
    enum struct UnaryOp {
        Plus,   // +a
        Minus,  // -a
        Not,    // !a
        Deref,  // *a
        Ref,    // &a
        Option, // ?a
    };

    enum struct BinaryOp {
        BitwiseAnd,         // a & b
        BitwiseOr,          // a | b
        BitwiseXor,         // a ^ b
        Add,                // a + b
        Subtract,           // a - b
        Multiply,           // a * b
        Divide,             // a / b
        Modulo,             // a % b
        CompEq,             // a == b
        CompNe,             // a != b
        CompGt,             // a > b
        CompGe,             // a >= b
        CompLt,             // a < b
        CompLe,             // a <= b
        LogicalAnd,         // a && b
        LogicalOr,          // a || b
        ShiftLeft,          // a << b
        ShiftRight,         // a >> b
        UnsignedShiftRight, // a >>> b
    };

    enum struct NodeType {
        None,

        #define WLANG_AST(X, C) X##C,
        #include <frontend/ast/node_list.hpp>
    };
}

#endif
