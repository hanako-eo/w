#if !defined(WLANG_ERROR)
#error You must define WLANG_ERROR before including this file
#endif

#define WLANG_PARSER_ERROR(name, message, ...) WLANG_ERROR(Parser ## name, message, __VA_ARGS__)

WLANG_PARSER_ERROR(ExpectedToken, "expected token {}, got {}", TokenKind, TokenKind)
WLANG_PARSER_ERROR(UnexpectedToken, "unexpected token {}", TokenKind)

#undef WLANG_PARSER_ERROR
#undef WLANG_ERROR
