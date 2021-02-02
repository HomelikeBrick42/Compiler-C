#pragma once

#include <cstdint>

struct Token;
enum struct TokenKind;

class Lexer {
public:
	Lexer(const char* source);
	Token* NextToken();
private:
	char Advance(uint64_t length = 1);
	Token* ReturnNext(TokenKind kind, uint64_t length);
	char Peek(uint64_t offset);
	Token* LexWhitespace();
	Token* LexIdentifier();
	Token* LexNumber();
	Token* LexBinary();
	Token* LexHexadecimal();
	Token* LexFloat();
	Token* LexSingleLineComment();
	Token* LexMultilineComment();
private:
	const char* m_Source;
	uint64_t m_SourceLength;
	uint64_t m_Position;
	char m_Current;
};
