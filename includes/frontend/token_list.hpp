#if !defined(WLANG_TOKEN)
#error You must define WLANG_TOKEN before including this file
#endif

WLANG_TOKEN(Unknown)
WLANG_TOKEN(Eof)
WLANG_TOKEN(Ident) // user
WLANG_TOKEN(Integer) // 123
WLANG_TOKEN(Float) // 123.4
WLANG_TOKEN(String) // 'foo'
// WLANG_TOKEN(StrInter) // 'name=$user.name'
WLANG_TOKEN(Rune) // `A` - rune
WLANG_TOKEN(Plus) // +
WLANG_TOKEN(Minus) // -
WLANG_TOKEN(Mul) // *
WLANG_TOKEN(Div) // /
WLANG_TOKEN(Mod) // %
WLANG_TOKEN(BitAnd) // &
WLANG_TOKEN(BitOr) // |
WLANG_TOKEN(BitXor) // ^
WLANG_TOKEN(Inc) // ++
WLANG_TOKEN(Dec) // --
WLANG_TOKEN(LogicalAnd) // &&
WLANG_TOKEN(LogicalOr) // ||
WLANG_TOKEN(Not) // !
WLANG_TOKEN(BitNot) // ~
WLANG_TOKEN(Question) // ?
WLANG_TOKEN(Comma) // ,
WLANG_TOKEN(Semicolon) // ;
WLANG_TOKEN(Colon) // :
WLANG_TOKEN(Hash) // #
// WLANG_TOKEN(Dollar) // $
WLANG_TOKEN(At) // @
WLANG_TOKEN(StrDollar)
WLANG_TOKEN(LeftShift) // <<
WLANG_TOKEN(RightShift) // >>
WLANG_TOKEN(UnsignedRightShift) // >>>
WLANG_TOKEN(NotIn) // !in
WLANG_TOKEN(NotIs) // !is
WLANG_TOKEN(Assign) // =
WLANG_TOKEN(DeclAssign) // :=
WLANG_TOKEN(PlusAssign) // +=
WLANG_TOKEN(MinusAssign) // -=
WLANG_TOKEN(DivAssign) // /=
WLANG_TOKEN(MultAssign) // *=
WLANG_TOKEN(XorAssign) // ^=
WLANG_TOKEN(ModAssign) // %=
WLANG_TOKEN(OrAssign) // |=
WLANG_TOKEN(AndAssign) // &=
WLANG_TOKEN(RightShiftAssign) // <<=
WLANG_TOKEN(LeftShiftAssign) // >>=
WLANG_TOKEN(UnsignedRightShiftAssign) // >>>=
WLANG_TOKEN(Lcbr) // {
WLANG_TOKEN(Rcbr) // }
WLANG_TOKEN(Lpar) // (
WLANG_TOKEN(Rpar) // )
WLANG_TOKEN(Nilsbr) // #[
WLANG_TOKEN(Lsbr) // [
WLANG_TOKEN(Rsbr) // ]
WLANG_TOKEN(Eq) // ==
WLANG_TOKEN(Ne) // !=
WLANG_TOKEN(Gt) // >
WLANG_TOKEN(Lt) // <
WLANG_TOKEN(Ge) // >=
WLANG_TOKEN(Le) // <=
WLANG_TOKEN(Comment)
WLANG_TOKEN(Dot) // .
WLANG_TOKEN(Dotdot) // ..
WLANG_TOKEN(Ellipsis) // ...
WLANG_TOKEN(KeyAs)
WLANG_TOKEN(KeyAsm)
WLANG_TOKEN(KeyAssert)
WLANG_TOKEN(KeyAtomic)
WLANG_TOKEN(KeyBreak)
WLANG_TOKEN(KeyConst)
WLANG_TOKEN(KeyContinue)
WLANG_TOKEN(KeyDefer)
WLANG_TOKEN(KeyElse)
WLANG_TOKEN(KeyEnum)
WLANG_TOKEN(KeyFalse)
WLANG_TOKEN(KeyFor)
WLANG_TOKEN(KeyFn)
WLANG_TOKEN(KeyGlobal)
WLANG_TOKEN(KeyGoto)
WLANG_TOKEN(KeyIf)
WLANG_TOKEN(KeyImport)
WLANG_TOKEN(KeyIn)
WLANG_TOKEN(KeyInterface)
WLANG_TOKEN(KeyIs)
WLANG_TOKEN(KeyMatch)
WLANG_TOKEN(KeyModule)
WLANG_TOKEN(KeyMut)
WLANG_TOKEN(KeyNil)
WLANG_TOKEN(KeyNone)
WLANG_TOKEN(KeyReturn)
WLANG_TOKEN(KeyLike)
WLANG_TOKEN(KeySizeof)
// WLANG_TOKEN(KeyIsreftype)
WLANG_TOKEN(KeyLikely)
WLANG_TOKEN(KeyUnlikely)
WLANG_TOKEN(KeyOffsetof)
WLANG_TOKEN(KeyStruct)
WLANG_TOKEN(KeyTrue)
WLANG_TOKEN(KeyType)
WLANG_TOKEN(KeyTypeof)
WLANG_TOKEN(KeyDump)
WLANG_TOKEN(KeyOr)
WLANG_TOKEN(KeyUnion)
WLANG_TOKEN(KeyPub)
WLANG_TOKEN(KeyStatic)
WLANG_TOKEN(KeyVolatile)
WLANG_TOKEN(KeyUnsafe)

#undef WLANG_TOKEN
