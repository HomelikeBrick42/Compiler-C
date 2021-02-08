#pragma once

#include "Typedefs.h"

DECLARE_STRUCT(Token);

typedef struct Lexer {
	const char* Source;
	u64 SourceLength;
	char Current;
	u64 Position;
	u64 Line;
	u64 CharLinePosition;
} Lexer;

Lexer* CreateLexer(const char* source);
Token* LexerNextToken(Lexer* lexer);

char LexerPeek(Lexer* lexer, u64 offset);
char LexerNext(Lexer* lexer);

Token* LexerWhitespace(Lexer* lexer);
Token* LexerIdentifier(Lexer* lexer);
Token* LexerSingleLineComment(Lexer* lexer);
Token* LexerMultilineComment(Lexer* lexer);
Token* LexerInt(Lexer* lexer);
Token* LexerFloat(Lexer* lexer);
