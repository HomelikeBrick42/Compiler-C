#pragma once

#include "Typedefs.h"

typedef enum TokenKind {
	TOKEN_KIND_END_OF_FILE = 0,

	TOKEN_KIND_END_OF_SINGLE_CHARS = 128,

	// Double chars
	TOKEN_KIND_RIGHT_ARROW,
	TOKEN_KIND_PLUS_EQUALS,
	TOKEN_KIND_MINUS_EQUALS,
	TOKEN_KIND_ASTERISKE_QUALS,
	TOKEN_KIND_FORWARD_SLASH_EQUALS,
	TOKEN_KIND_PERCENT_EQUALS,
	TOKEN_KIND_CARET_EQUALS,
	TOKEN_KIND_COLON_COLON,
	TOKEN_KIND_COLON_EQUALS,
	TOKEN_KIND_EQUALS_EQUALS,
	TOKEN_KIND_EXCLAMATIONMARK_EQUALS,
	TOKEN_KIND_AMPERSAND_AMPERSAND,
	TOKEN_KIND_AMPERSAND_EQUALS,
	TOKEN_KIND_PIPE_PIPE,
	TOKEN_KIND_PIPE_EQUALS,
	TOKEN_KIND_LESSTHAN_LESSTHAN,
	TOKEN_KIND_GREATERTHAN_GREATERTHAN,
	TOKEN_KIND_LESSTHAN_EQUALS,
	TOKEN_KIND_GREATERTHAN_EQUALS,

	// Triple Chars
	TOKEN_KIND_LESSTHAN_LESSTHAN_EQUALS,
	TOKEN_KIND_GREATERTHAN_GREATERTHAN_EQUALS,

	// Misc
	TOKEN_KIND_WHITESPACE = 256,
	TOKEN_KIND_COMMENT,
	TOKEN_KIND_IDENTIFIER,
	TOKEN_KIND_INT_LITERAL,
	TOKEN_KIND_FLOAT_LITERAL,

	TOKEN_KIND_COUNT,
} TokenKind;

void PrintTokenKind(TokenKind kind);
