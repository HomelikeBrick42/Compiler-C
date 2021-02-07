#include "Parser.hpp"
#include "Node.hpp"
#include "NodeKind.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"
#include "Lexer.hpp"
#include "DynamicArray.hpp"

#include <cassert>

/*
	3 * 4 + 5;

	* priority is 100
	+ priority is 50

	parse_expression(-9999)
		3
		* means recursively call parse_expression
		priority of * is 100
		100 > -9999, so recurse
		parse_expression(100)
			4
			+ means recursively call parse_expression
			priority of + is 50
			50 < 100, so DO NOT RECURSE
			instead return 4

		left = 3 * 4
		we see +
		+ means recursively call parse_expression
		priority of + is 50
		50 > -9999, so recurse
		parse_expression(50)
			5
			we see ;
			; means DO NOT RECURSE
			return 5

		left = 3 * 3 + 5
		we see ;
		; means DO NOT RECURSE
		return 3 * 4 + 5
*/

Parser::Parser(const char* source)
	: m_Source(source), m_Tokens(nullptr), m_Position(0), m_Current(nullptr) {
	m_Tokens = DynamicArrayCreate(Token*);
	Lexer lexer(m_Source);
	while (true) {
		Token* token = lexer.NextToken();
		if (token->Kind != TokenKind::Comment &&
			token->Kind != TokenKind::Whitespace) {
			DynamicArrayPush(m_Tokens, token);
		}
		if (token->Kind == TokenKind::EndOfFile) {
			break;
		}
	}
	m_TokensLength = DynamicArrayGetLength(m_Tokens);
	m_Current = m_Tokens[m_Position];
}

Parser::~Parser() {
	for (int i = 0; i < DynamicArrayGetLength(m_Tokens); i++) {
		delete m_Tokens[i];
	}
	DynamicArrayDestroy(m_Tokens);
}

Node* Parser::Parse() {
	Node* root = ParseExpression();
	assert(EatToken(TokenKind::Semicolon));
	return root;
}

Token* Parser::Peek(uint64_t offset) {
	uint64_t index = m_Position + offset;
	if (index >= m_TokensLength)
		return m_Tokens[m_TokensLength - 1];
	return m_Tokens[index];
}

Token* Parser::NextToken() {
	Token* token = m_Current;
	m_Position++;
	if (m_Position >= m_TokensLength) {
		m_Current = m_Tokens[m_TokensLength - 1];
	}
	else {
		m_Current = m_Tokens[m_Position];
	}
	return token;
}

bool Parser::MatchToken(TokenKind kind) {
	if (m_Current->Kind == kind)
		return true;
	return false;
}

bool Parser::EatToken(TokenKind kind) {
	if (m_Current->Kind == kind) {
		NextToken();
		return true;
	}
	return false;
}

Node* Parser::ParseExpression() {
	return ParseBinaryExpression();
}

Node* Parser::ParsePrimaryExpression() {
	switch (m_Current->Kind) {
	case TokenKind::Int: {
		Token* token = NextToken();
		return new IntNode{ NodeKind::Int, static_cast<IntToken*>(token)->Value };
	}
	case TokenKind::Float: {
		Token* token = NextToken();
		return new FloatNode{ NodeKind::Float, static_cast<FloatToken*>(token)->Value };
	}
	case TokenKind::OpenParentheses: {
		assert(EatToken(TokenKind::OpenParentheses));
		Node* expression = ParseBinaryExpression();
		assert(EatToken(TokenKind::CloseParentheses));
		return expression;
	}
	default:
		return ParseBinaryExpression();
	}
}

Node* Parser::ParseBinaryExpression(uint64_t parentPresedence) {
	Node* left;
	uint64_t unaryOperatorPrecedence = GetUnaryOperatorPrecedence(m_Current->Kind);
	if (unaryOperatorPrecedence != 0 && unaryOperatorPrecedence >= parentPresedence) {
		Token* operatorToken = NextToken();
		Node* operand = ParseBinaryExpression(unaryOperatorPrecedence);
		left = new UnaryOperatorNode{ NodeKind::UnaryOperator, operatorToken, operand };
	}
	else {
		left = ParsePrimaryExpression();
	}

	while (true) {
		uint64_t precedence = GetBinaryOperatorPrecedence(m_Current->Kind);
		if (precedence == 0 || precedence <= parentPresedence)
			break;

		Token* operatorToken = NextToken();
		Node* right = ParseBinaryExpression(precedence);
		left = new BinaryOperatorNode{ NodeKind::BinaryOperator, left, operatorToken, right };
	}

	return left;
}

uint64_t Parser::GetUnaryOperatorPrecedence(TokenKind kind) {
	switch (kind)
	{
	case TokenKind::ExclamationMark:
	case TokenKind::Asterisk:
		return 7;

	case TokenKind::Plus:
	case TokenKind::Minus:
		return 6;

	default:
		return 0;
	}
}

uint64_t Parser::GetBinaryOperatorPrecedence(TokenKind kind) {
	switch (kind)
	{
	// Multiplication
	case TokenKind::LessThanLessThan:
	case TokenKind::GreaterThanGreaterThan:
	case TokenKind::Caret:
	case TokenKind::Ampersand:
	case TokenKind::Asterisk:
	case TokenKind::ForwardSlash:
	case TokenKind::Percent:
		return 5;

	// Addition
	case TokenKind::Pipe:
	case TokenKind::Plus:
	case TokenKind::Minus:
		return 4;

	// Compare
	case TokenKind::LessThan:
	case TokenKind::GreaterThan:
	case TokenKind::LessThanEquals:
	case TokenKind::GreaterThanEquals:
	case TokenKind::EqualsEquals:
	case TokenKind::ExclamationMarkEquals:
		return 3;

	// And
	case TokenKind::AmpersandAmpersand:
		return 2;

	// Or
	case TokenKind::PipePipe:
		return 1;

	default:
		return 0;
	}
}
