#ifndef DIAG
#define DIAG(ID, KIND, MSG)
#endif

/// lexer
DIAG(err_unknown_char, Error, "unknown char '{0}'")
DIAG(err_unclosed_character, Error, "unclosed character literal")
DIAG(err_unclosed_string, Error, "unclosed string literal")
DIAG(err_numeric_constant, Error, "invalid numeric constant")

/// parser
DIAG(err_expected, Error, "expected '{0}'")

#undef DIAG