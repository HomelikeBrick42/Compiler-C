#pragma once

#include "Typedefs.h"

DECLARE_ENUM(TokenKind);

typedef struct Token {
	TokenKind Kind;
	u64 Line;
	u64 CharLinePosition;
	const char* Source;
	union {
		const char* Name;
		u64 IntValue;
		f64 FloatValue;
	};
} Token;

Token* CreateToken(TokenKind kind, u64 line, u64 charLinePosition, const char* source);
Token* CreateIdentifierToken(const char* name, u64 line, u64 charLinePosition, const char* source);
Token* CreateIntToken(u64 value, u64 line, u64 charLinePosition, const char* source);
Token* CreateFloatToken(double value, u64 line, u64 charLinePosition, const char* source);

void PrintToken(Token* token);
