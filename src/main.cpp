#include <iostream>

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <frontend/lexer.hpp>

using namespace std;

constexpr auto s_operations = frozen::make_unordered_map<frozen::string, W::TokenKind>({
        { ">>>=", W::TokenKind::UnsignedRightShiftAssign },
        { ">>>", W::TokenKind::UnsignedRightShift },
        { "<<=", W::TokenKind::RightShiftAssign },
        { "<<", W::TokenKind::LeftShift },
        { ">>=", W::TokenKind::LeftShiftAssign },
        { ">>", W::TokenKind::RightShift },
        { ">=", W::TokenKind::Ge },
        { ">", W::TokenKind::Gt },
        { "<=", W::TokenKind::Le },
        { "<", W::TokenKind::Lt },
        { "==", W::TokenKind::Eq },
        { "!=", W::TokenKind::Ne },
        { ":=", W::TokenKind::DeclAssign },
        { "+=", W::TokenKind::PlusAssign },
        { "-=", W::TokenKind::MinusAssign },
        { "/=", W::TokenKind::DivAssign },
        { "*=", W::TokenKind::MultAssign },
        { "^=", W::TokenKind::XorAssign },
        { "%=", W::TokenKind::ModAssign },
        { "|=", W::TokenKind::OrAssign },
        { "&=", W::TokenKind::AndAssign },
        { "=", W::TokenKind::Assign },
        { "!in", W::TokenKind::NotIn },
        { "!is", W::TokenKind::NotIs },
        { "!", W::TokenKind::Not },
        { "++", W::TokenKind::Inc },
        { "+", W::TokenKind::Plus },
        { "--", W::TokenKind::Dec },
        { "-", W::TokenKind::Minus },
        { "*", W::TokenKind::Mul },
        { "/", W::TokenKind::Div },
        { "%", W::TokenKind::Mod },
        { "^", W::TokenKind::Xor },
        { "~", W::TokenKind::BitNot },
        { "?", W::TokenKind::Question },
        { ",", W::TokenKind::Comma },
        { ";", W::TokenKind::Semicolon },
        { ":", W::TokenKind::Colon },
        { "&&", W::TokenKind::And },
        { "&", W::TokenKind::Amp },
        { "||", W::TokenKind::LogicalOr },
        { "|", W::TokenKind::Pipe },
        { "$", W::TokenKind::Dollar },
        { "@", W::TokenKind::At },
        { "{", W::TokenKind::Lcbr },
        { "}", W::TokenKind::Rcbr },
        { "(", W::TokenKind::Lpar },
        { ")", W::TokenKind::Rpar },
        { "#[", W::TokenKind::Nilsbr },
        { "[", W::TokenKind::Lsbr },
        { "]", W::TokenKind::Rsbr },
        { "#", W::TokenKind::Hash },
        { "...", W::TokenKind::Ellipsis },
        { "..", W::TokenKind::Dotdot },
        { ".", W::TokenKind::Dot },
    });

int main(int argc, char **argv) {
    for (auto op : s_operations) {
        cout << op.first.data() << endl;
    }
    return 0;
}
