#include "Token.h"
#include "TokenKind.h"
#include "Alloc.h"

#include <stdio.h>

Token* CreateToken(TokenKind kind, u64 line, u64 charLinePosition, const char* source) {
	Token* token = (Token*)xcalloc(1, sizeof(Token));
	token->Kind = kind;
	token->Line = line;
	token->CharLinePosition = charLinePosition;
	token->Source = source;
	return token;
}

Token* CreateIdentifierToken(const char* name, u64 line, u64 charLinePosition, const char* source) {
	Token* token = (Token*)xcalloc(1, sizeof(Token));
	token->Kind = TOKEN_KIND_IDENTIFIER;
	token->Line = line;
	token->CharLinePosition = charLinePosition;
	token->Source = source;
	token->Name = name;
	return token;
}

Token* CreateIntToken(u64 value, u64 line, u64 charLinePosition, const char* source) {
	Token* token = (Token*)xcalloc(1, sizeof(Token));
	token->Kind = TOKEN_KIND_INT_LITERAL;
	token->Line = line;
	token->CharLinePosition = charLinePosition;
	token->Source = source;
	token->IntValue = value;
	return token;
}

Token* CreateFloatToken(double value, u64 line, u64 charLinePosition, const char* source) {
	Token* token = (Token*)xcalloc(1, sizeof(Token));
	token->Kind = TOKEN_KIND_FLOAT_LITERAL;
	token->Line = line;
	token->CharLinePosition = charLinePosition;
	token->Source = source;
	token->FloatValue = value;
	return token;
}

void PrintToken(Token* token) {
	printf("Token Kind: '");
	PrintTokenKind(token->Kind);
	printf("'");
	switch (token->Kind) {
	case TOKEN_KIND_IDENTIFIER:
		printf(", Name: '%s'", token->Name);
		break;
	case TOKEN_KIND_INT_LITERAL:
		printf(", Value: %llu", token->IntValue);
		break;
	case TOKEN_KIND_FLOAT_LITERAL:
		printf(", Value: %f", token->FloatValue);
		break;
	}
	printf(" Line: %llu, Position: %llu\n", token->Line, token->CharLinePosition);
}
