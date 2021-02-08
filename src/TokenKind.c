#include "TokenKind.h"

#include <stdio.h>

const char* TokenKindNames[TOKEN_KIND_COUNT] = {
	[TOKEN_KIND_END_OF_FILE] = "End Of File",

	// Double chars
	[TOKEN_KIND_RIGHT_ARROW] = "->",
	[TOKEN_KIND_PLUS_EQUALS] = "+=",
	[TOKEN_KIND_MINUS_EQUALS] = "-=",
	[TOKEN_KIND_ASTERISK_EQUALS] = "*=",
	[TOKEN_KIND_FORWARD_SLASH_EQUALS] = "/=",
	[TOKEN_KIND_PERCENT_EQUALS] = "%=",
	[TOKEN_KIND_CARET_EQUALS] = "^=",
	[TOKEN_KIND_COLON_COLON] = "::",
	[TOKEN_KIND_COLON_EQUALS] = ":=",
	[TOKEN_KIND_EQUALS_EQUALS] = "==",
	[TOKEN_KIND_EXCLAMATIONMARK_EQUALS] = "!=",
	[TOKEN_KIND_AMPERSAND_AMPERSAND] = "&&",
	[TOKEN_KIND_AMPERSAND_EQUALS] = "&=",
	[TOKEN_KIND_PIPE_PIPE] = "||",
	[TOKEN_KIND_PIPE_EQUALS] = "|=",
	[TOKEN_KIND_LESSTHAN_LESSTHAN] = "<<",
	[TOKEN_KIND_GREATERTHAN_GREATERTHAN] = ">>",
	[TOKEN_KIND_LESSTHAN_EQUALS] = "<=",
	[TOKEN_KIND_GREATERTHAN_EQUALS] = ">=",

	// Triple Chars
	[TOKEN_KIND_LESSTHAN_LESSTHAN_EQUALS] = "<<=",
	[TOKEN_KIND_GREATERTHAN_GREATERTHAN_EQUALS] = ">>=",

	// Misc
	[TOKEN_KIND_WHITESPACE] = "Whitespace",
	[TOKEN_KIND_COMMENT] = "Comment",
	[TOKEN_KIND_IDENTIFIER] = "Identifier",
	[TOKEN_KIND_INT_LITERAL] = "Int Literal",
	[TOKEN_KIND_FLOAT_LITERAL] = "Float Literal",
};

void PrintTokenKind(TokenKind kind) {
	if (kind > 0 && kind < 128) {
		printf("%c", (char)kind);
		return;
	}
	else if (kind >= 0 && kind < TOKEN_KIND_COUNT) {
		const char* name = TokenKindNames[kind];
		if (name) {
			printf("%s", name);
			return;
		}
	}
	printf("TokenKindError");
	return;
}
