#include "Typedefs.h"
#include "TokenKind.h"
#include "Token.h"
#include "Lexer.h"
#include "TypeInfo.h"
#include "File.h"
#include "StringIntern.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Not enough arguments!\n");
		return 1;
	}

	char* source = LoadFile(argv[1]);
	if (source == nullptr) {
		printf("Unable to open file '%s'\n", argv[1]);
		return 2;
	}

	Lexer* lexer = CreateLexer(source);
	bool end = false;
	while (!end) {
		Token* token = LexerNextToken(lexer);
		if (token->Kind == TOKEN_KIND_END_OF_FILE) {
			end = true;
		}

		PrintToken(token);
		free(token);
	}

	free(lexer);
	free(source);

	return 0;
}
