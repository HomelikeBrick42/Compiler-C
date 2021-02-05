#pragma once

#include <cstdint>

enum struct TokenKind;

struct Token {
	TokenKind Kind;
	uint64_t Position;
	uint64_t Length;
};

struct IdentifierToken : Token {
	const char* Name;
};

struct IntToken : Token {
	uint64_t Value;
};

struct FloatToken : Token {
	double Value;
};

void PrintToken(const char* source, Token* token);
