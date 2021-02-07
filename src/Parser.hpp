#pragma once

#include <cstdint>

struct Token;
enum struct TokenKind : uint64_t;
struct Node;

class Parser {
public:
	Parser(const char* source);
	~Parser();
	Node* Parse();
private:
	Token* Peek(uint64_t offset);
	Token* NextToken();
	bool MatchToken(TokenKind kind);
	bool EatToken(TokenKind kind);

	Node* ParseExpression();
	Node* ParsePrimaryExpression();
	Node* ParseBinaryExpression(uint64_t parentPresedence = 0);

	Node* ParseStatement();
private:
	uint64_t GetUnaryOperatorPrecedence(TokenKind kind);
	uint64_t GetBinaryOperatorPrecedence(TokenKind kind);
private:
	const char* m_Source;
	Token** m_Tokens;
	uint64_t m_TokensLength;
	uint64_t m_Position;
	Token* m_Current;
};
