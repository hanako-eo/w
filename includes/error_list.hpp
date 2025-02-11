#if !defined(WLANG_ERROR)
#error You must define WLANG_ERROR before including this file
#endif

#define WLANG_PARSER_ERROR(name, message, ...) WLANG_ERROR(Parser ## name, message, __VA_ARGS__)

WLANG_PARSER_ERROR(ExpectedToken, "expected token {}, got {}", TokenKind, TokenKind)
WLANG_PARSER_ERROR(UnexpectedToken, "unexpected token {}", TokenKind)
WLANG_PARSER_ERROR(EmptyRune, "a rune cannot be empty")
WLANG_PARSER_ERROR(RuneIsNotAString, "the rune `{}` is not a string, please replace ` by \"", std::string)
WLANG_PARSER_ERROR(IllFormedRune, "invalid rune `{}`", std::string)
WLANG_PARSER_ERROR(UnsupportedPrefixDec, "prefix decrementation is unsupported")
WLANG_PARSER_ERROR(UnsupportedPrefixInc, "prefix incrementation is unsupported")

#undef WLANG_PARSER_ERROR
#undef WLANG_ERROR
