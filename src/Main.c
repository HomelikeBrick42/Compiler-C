#include "Typedefs.h"
#include "TokenKind.h"
#include "Token.h"
#include "Lexer.h"
#include "TypeInfo.h"
#include "File.h"
#include "StringIntern.h"
#include "DynamicArray.h"

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

	Token** tokens = DynamicArrayCreate(Token*);
	{
		Lexer* lexer = CreateLexer(source);

		bool end = false;
		while (!end) {
			Token* token = LexerNextToken(lexer);
			if (token->Kind == TOKEN_KIND_END_OF_FILE) {
				end = true;
			}

			DynamicArrayPush(tokens, token);
		}

		free(lexer);
	}

	ForArray(tokens) {
		PrintToken(tokens[i]);
	}

	{
		ForArray(tokens) {
			free(tokens[i]);
		}
		DynamicArrayDestroy(tokens);

		free(source);
	}

	return 0;
}
