#include "Lexer.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"

#include <cassert>
#include <cstring>
#include <cstdlib>

Lexer::Lexer(const char* source)
	: m_Source(source), m_SourceLength(strlen(m_Source)), m_Position(0), m_Current(m_Source[m_Position]) {}

#define LEX_MULTI_1(c1, k1, c2, k2) \
	case c1: { \
		if (Peek(1) == c2) { \
			return ReturnNext(k2, 2); \
		} \
		return ReturnNext(k1, 1); \
	} break

#define LEX_MULTI_2(c1, k1, c2, k2, c3, k3) \
	case c1: { \
		if (Peek(1) == c2) { \
			return ReturnNext(k2, 2); \
		} else if (Peek(1) == c3) { \
			return ReturnNext(k3, 2); \
		} \
		return ReturnNext(k1, 1); \
	} break

Token* Lexer::NextToken() {
	switch (m_Current) {
		case ' ': case '\n': case '\r':
		case '\t': case '\v': case '\f':
			return LexWhitespace();
		case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o':
		case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y':
		case 'z': case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S':
		case 'T': case 'U': case 'V': case 'W': case 'X':
		case 'Y': case 'Z': case '_':
			return LexIdentifier();
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return LexNumber();

		case '<':
		{
			if (Peek(1) == '<') {
				if (Peek(2) == '=') {
					return ReturnNext(TokenKind::LessThanLessThanEquals, 3);
				}
				return ReturnNext(TokenKind::LessThanLessThan, 2);
			} else if (Peek(2) == '=') {
				return ReturnNext(TokenKind::LessThanEquals, 2);
			}
			return ReturnNext(TokenKind::LessThan, 1);
		} break;

		case '>':
		{
			if (Peek(1) == '>') {
				if (Peek(2) == '=') {
					return ReturnNext(TokenKind::GreaterThanGreaterThanEquals, 3);
				}
				return ReturnNext(TokenKind::GreaterThanGreaterThan, 2);
			} else if (Peek(2) == '=') {
				return ReturnNext(TokenKind::GreaterThanEquals, 2);
			}
			return ReturnNext(TokenKind::GreaterThan, 1);
		} break;

		case '/': {
			if (Peek(1) == '/') {
				return LexSingleLineComment();
			} else if (Peek(1) == '*') {
				return LexMultilineComment();
			} else if (Peek(1) == '=') {
				return ReturnNext(TokenKind::ForwardSlashEquals, 2);
			}
			return ReturnNext(TokenKind::ForwardSlash, 1);
		} break;

		LEX_MULTI_1('+', TokenKind::Plus,            '=', TokenKind::PlusEquals);
		LEX_MULTI_1('*', TokenKind::Asterisk,        '=', TokenKind::AsteriskEquals);
		LEX_MULTI_1('%', TokenKind::Percent,         '=', TokenKind::PercentEquals);
		LEX_MULTI_1('^', TokenKind::Caret,           '=', TokenKind::CaretEquals);
		LEX_MULTI_1('=', TokenKind::Equals,          '=', TokenKind::EqualsEquals);
		LEX_MULTI_1('!', TokenKind::ExclamationMark, '=', TokenKind::ExclamationMarkEquals);

		LEX_MULTI_2('-', TokenKind::Minus,     '=', TokenKind::MinusEquals,         '>', TokenKind::RightArrow);
		LEX_MULTI_2('&', TokenKind::Ampersand, '&', TokenKind::AmpersandAmpersand,  '=', TokenKind::AmpersandEquals);
		LEX_MULTI_2('|', TokenKind::Pipe,      '|', TokenKind::PipePipe,            '=', TokenKind::PipeEquals);
		LEX_MULTI_2(':', TokenKind::Colon,     ':', TokenKind::ColonColon,          '=', TokenKind::ColonEquals);

		case '\0': return ReturnNext(TokenKind::EndOfFile,          0);
		case '(':  return ReturnNext(TokenKind::OpenParentheses,    1);
		case ')':  return ReturnNext(TokenKind::CloseParentheses,   1);
		case '[':  return ReturnNext(TokenKind::OpenSquareBracket,  1);
		case ']':  return ReturnNext(TokenKind::CloseSquareBracket, 1);
		case '{':  return ReturnNext(TokenKind::OpenBracket,        1);
		case '}':  return ReturnNext(TokenKind::CloseBracket,       1);
		case ';':  return ReturnNext(TokenKind::Semicolon,          1);
		case '?':  return ReturnNext(TokenKind::QuestionMark,       1);
		case ',':  return ReturnNext(TokenKind::Comma,              1);
		case '`':  return ReturnNext(TokenKind::Grave,              1);
		case '.':  return ReturnNext(TokenKind::Dot,				1);
		default:   return ReturnNext(TokenKind::Bad,				1);
	}
}

#undef LEX_MULTI

char Lexer::Advance(uint64_t length) {
	char next = m_Current;
	m_Position += length;
	m_Current = Peek(0);
	return next;
}

Token* Lexer::ReturnNext(TokenKind kind, uint64_t length) {
	Token* token = new Token{ kind, m_Position, length };
	Advance(length);
	return token;
}

char Lexer::Peek(uint64_t offset) {
	uint64_t index = m_Position + offset;
	if (index >= m_SourceLength)
		return '\0';
	return m_Source[index];
}

Token* Lexer::LexWhitespace() {
	uint64_t start = m_Position;
	uint64_t length = 0;

	while (true) {
		switch (m_Current) {
		case ' ': case '\n': case '\r':
		case '\t': case '\v': case '\f':
			length++;
			Advance();
			continue;
		default:
			break;
		}
		break;
	}

	return new Token{ TokenKind::Whitespace, start, length };
}

Token* Lexer::LexIdentifier() {
	uint64_t start = m_Position;
	uint64_t length = 0;

	while (true) {
		switch (m_Current) {
		case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o':
		case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y':
		case 'z': case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S':
		case 'T': case 'U': case 'V': case 'W': case 'X':
		case 'Y': case 'Z': case '_': case '0': case '1':
		case '2': case '3': case '4': case '5': case '6':
		case '7': case '8': case '9':
			length++;
			Advance();
			continue;
		default:
			break;
		}
		break;
	}

	return new Token{ TokenKind::Identifier, start, length };
}

Token* Lexer::LexNumber() {
	uint64_t start = m_Position;
	uint64_t length = 0;
	uint64_t value = 0;

	if (m_Current == '0') {
		switch (Peek(1)) {
		case 'b': case 'B':
			return LexBinary();
		case 'x': case 'X':
			return LexHexadecimal();
		default:
			break;
		}
	}

	while (true) {
		switch (Peek(length)) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			length++;
			continue;
		case '.':
			return LexFloat();
		default:
			break;
		}
		length = 0;
		break;
	}

	while (true)
	{
		switch (m_Current) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			length++;
			value *= 10;
			value += static_cast<uint64_t>(m_Current - '0');
			Advance();
			continue;
		default:
			break;
		}
		break;
	}

	return new IntToken{ TokenKind::Int, start, length, value };
}

Token* Lexer::LexBinary() {
	uint64_t start = m_Position;
	uint64_t length = 0;
	uint64_t value = 0;
	assert(false); // @Incomplete: Not implemented
	return new IntToken{ TokenKind::Int, start, length, value };
}

Token* Lexer::LexHexadecimal() {
	uint64_t start = m_Position;
	uint64_t length = 0;
	uint64_t value = 0;
	assert(false); // @Incomplete: Not implemented
	return new IntToken{ TokenKind::Int, start, length, value };
}

Token* Lexer::LexFloat() {
	uint64_t start = m_Position;

	char* end;
	double value = strtod(&m_Source[start], &end);
	uint64_t length = (uint64_t)(end - &m_Source[start]);
	Advance(length);

	return new FloatToken{ TokenKind::Float, start, length, value };
}

Token* Lexer::LexSingleLineComment() {
	uint64_t start = m_Position;
	uint64_t length = 0;

	while (m_Current != '\n' && m_Current != '\0') {
		length++;
		Advance();
	}

	return new Token{ TokenKind::Comment, start, length };
}

Token* Lexer::LexMultilineComment() {
	uint64_t start = m_Position;
	uint64_t length = 0;
	uint64_t depth = 0;

	while (m_Current != '\0') {
		if (Peek(0) == '/' && Peek(1) == '*') {
			depth++;
			length += 2;
			Advance(2);
		} else if (Peek(0) == '*' && Peek(1) == '/') {
			depth--;
			length += 2;
			Advance(2);
			if (depth == 0)
				break;
		} else {
			length++;
			Advance();
		}
	}

	assert(depth == 0); // @Incomplete: Implement actuall error message

	return new Token{ TokenKind::Comment, start, length };
}
