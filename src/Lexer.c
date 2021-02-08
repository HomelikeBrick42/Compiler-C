#include "Lexer.h"
#include "TokenKind.h"
#include "Token.h"
#include "Alloc.h"
#include "StringIntern.h"
#include "DynamicArray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Lexer* CreateLexer(const char* source) {
	Lexer* lexer = (Lexer*)xcalloc(1, sizeof(Lexer));
    lexer->Source = source;
    lexer->SourceLength = strlen(lexer->Source);
    lexer->Position = 0;
    lexer->CharLinePosition = 0;
    lexer->Line = 1;
    lexer->Current = lexer->Source[lexer->Position];
	return lexer;
}

#define MATCH_1(c, c1, k1) \
	case c: { \
		switch (LexerPeek(lexer, 1)) { \
		case c1: \
			LexerNext(lexer); \
			LexerNext(lexer); \
			return CreateToken(k1, 2, lexer->Line, lexer->CharLinePosition, lexer->Source); \
		default: \
			goto Default; \
		} \
	}

#define MATCH_2(c, c1, k1, c2, k2) \
	case c: { \
		switch (LexerPeek(lexer, 1)) { \
		case c1: \
			LexerNext(lexer); \
			LexerNext(lexer); \
			return CreateToken(k1, 2, lexer->Line, lexer->CharLinePosition, lexer->Source); \
		case c2: \
			LexerNext(lexer); \
			LexerNext(lexer); \
			return CreateToken(k2, 2, lexer->Line, lexer->CharLinePosition, lexer->Source); \
		default: \
			goto Default; \
		} \
	}

Token* LexerNextToken(Lexer* lexer) {
    switch (lexer->Current) {
    case ' ':  case '\t': case '\n':
    case '\r': case '\v': case '\f':
        return LexerWhitespace(lexer);
	case 'a': case 'b': case 'c': case 'd': case 'e':
    case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o':
    case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E':
    case 'F': case 'G': case 'H': case 'I': case 'J':
    case 'K': case 'L': case 'M': case 'N': case 'O':
    case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case '_':
        return LexerIdentifier(lexer);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return LexerInt(lexer);

	case '<': {
		if (LexerPeek(lexer, 1) == '<') {
			LexerNext(lexer);
			LexerNext(lexer);
			if (LexerPeek(lexer, 2) == '=') {
				LexerNext(lexer);
				return CreateToken(TOKEN_KIND_LESSTHAN_LESSTHAN_EQUALS, 3, lexer->Line, lexer->CharLinePosition, lexer->Source);
			}
			return CreateToken(TOKEN_KIND_LESSTHAN_LESSTHAN, 2, lexer->Line, lexer->CharLinePosition, lexer->Source);
		} else if (LexerPeek(lexer, 2) == '=') {
			LexerNext(lexer);
			LexerNext(lexer);
			return CreateToken(TOKEN_KIND_LESSTHAN_EQUALS, 2, lexer->Line, lexer->CharLinePosition, lexer->Source);
		}
		goto Default;
	}

	case '>': {
		if (LexerPeek(lexer, 1) == '>') {
			LexerNext(lexer);
			LexerNext(lexer);
			if (LexerPeek(lexer, 2) == '=') {
				LexerNext(lexer);
				return CreateToken(TOKEN_KIND_GREATERTHAN_GREATERTHAN_EQUALS, 3, lexer->Line, lexer->CharLinePosition, lexer->Source);
			}
			return CreateToken(TOKEN_KIND_GREATERTHAN_GREATERTHAN, 2, lexer->Line, lexer->CharLinePosition, lexer->Source);
		}
		else if (LexerPeek(lexer, 2) == '=') {
			LexerNext(lexer);
			LexerNext(lexer);
			return CreateToken(TOKEN_KIND_GREATERTHAN_EQUALS, 2, lexer->Line, lexer->CharLinePosition, lexer->Source);
		}
		goto Default;
	}

	case '/': {
		if (LexerPeek(lexer, 1) == '/') {
			return LexerSingleLineComment(lexer);
		} else if (LexerPeek(lexer, 1) == '*') {
			return LexerMultilineComment(lexer);
		} else if (LexerPeek(lexer, 1) == '=') {
			LexerNext(lexer);
			LexerNext(lexer);
			return CreateToken(TOKEN_KIND_FORWARD_SLASH_EQUALS, 2, lexer->Line, lexer->CharLinePosition, lexer->Source);
		}
		goto Default;
	}

	MATCH_1('+', '=', TOKEN_KIND_PLUS_EQUALS);
	MATCH_1('*', '=', TOKEN_KIND_ASTERISK_EQUALS);
	MATCH_1('%', '=', TOKEN_KIND_PERCENT_EQUALS);
	MATCH_1('^', '=', TOKEN_KIND_CARET_EQUALS);
	MATCH_1('=', '=', TOKEN_KIND_EQUALS_EQUALS);
	MATCH_1('!', '=', TOKEN_KIND_EQUALS_EQUALS);

	MATCH_2('-', '>', TOKEN_KIND_RIGHT_ARROW,			'=', TOKEN_KIND_MINUS_EQUALS);
	MATCH_2(':', ':', TOKEN_KIND_COLON_COLON,			'=', TOKEN_KIND_COLON_EQUALS);
	MATCH_2('&', '&', TOKEN_KIND_AMPERSAND_AMPERSAND,	'=', TOKEN_KIND_AMPERSAND_EQUALS);
	MATCH_2('|', '|', TOKEN_KIND_PIPE_PIPE,				'=', TOKEN_KIND_PIPE_EQUALS);

	default: Default: {
		return CreateToken(LexerNext(lexer), 1, lexer->Line, lexer->CharLinePosition, lexer->Source);
	}
    }
}

#undef MATCH_1
#undef MATCH_2

char LexerPeek(Lexer* lexer, u64 offset) {
    u64 index = lexer->Position + offset;
    if (index >= lexer->SourceLength) {
        return '\0';
    }
    return lexer->Source[index];
}

char LexerNext(Lexer* lexer) {
    char current = lexer->Current;
    lexer->Position++;
    lexer->Current = LexerPeek(lexer, 0);
    lexer->CharLinePosition++;
    if (current == '\n') {
        lexer->Line++;
        lexer->CharLinePosition = 1;
    }
    return current;
}

Token* LexerWhitespace(Lexer* lexer) {
	u64 length = 0;

	while (true) {
		switch (lexer->Current) {
		case ' ': case '\n': case '\r':
		case '\t': case '\v': case '\f':
			length++;
			LexerNext(lexer);
			continue;
		default:
			break;
		}
		break;
	}

	return CreateToken(TOKEN_KIND_WHITESPACE, length, lexer->Line, lexer->CharLinePosition, lexer->Source);
}

Token* LexerIdentifier(Lexer* lexer) {
	u64 start = lexer->Position;
	u64 length = 0;

	while (true) {
		switch (lexer->Current) {
		case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o':
		case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D': case 'E':
		case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O':
		case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '_':
			length++;
			LexerNext(lexer);
			continue;
		default:
			break;
		}
		break;
	}

	return CreateIdentifierToken(StringIntern(&lexer->Source[start], length), length, lexer->Line, lexer->CharLinePosition, lexer->Source);
}

Token* LexerSingleLineComment(Lexer* lexer) {
	u64 length = 0;

	while (lexer->Current != '\n' && lexer->Current != '\0') {
		length++;
		LexerNext(lexer);
	}

	return CreateToken(TOKEN_KIND_COMMENT, length, lexer->Line, lexer->CharLinePosition, lexer->Source);
}

Token* LexerMultilineComment(Lexer* lexer) {
	u64 length = 0;
	u64 depth = 0;

	while (lexer->Current != '\0') {
		if (LexerPeek(lexer, 0) == '/' && LexerPeek(lexer, 1) == '*') {
			depth++;
			length += 2;
			LexerNext(lexer);
			LexerNext(lexer);
		}
		else if (LexerPeek(lexer, 0) == '*' && LexerPeek(lexer, 1) == '/') {
			depth--;
			length += 2;
			LexerNext(lexer);
			LexerNext(lexer);
			if (depth == 0)
				break;
		}
		else {
			length++;
			LexerNext(lexer);
		}
	}

	assert(depth == 0); // @Incomplete: Implement actual error message

	return CreateToken(TOKEN_KIND_COMMENT, length, lexer->Line, lexer->CharLinePosition, lexer->Source);
}

static u64 CharToNumber[256] = {
    ['0'] = 0,
	['1'] = 1,
	['2'] = 2,
	['3'] = 3,
	['4'] = 4,
	['5'] = 5,
	['6'] = 6,
	['7'] = 7,
	['8'] = 8,
	['9'] = 9,
	['A'] = 10, ['a'] = 10,
	['B'] = 11, ['b'] = 11,
	['C'] = 12, ['c'] = 12,
	['D'] = 13, ['d'] = 13,
	['E'] = 14, ['e'] = 14,
	['F'] = 15, ['f'] = 15,
	['G'] = 16, ['g'] = 16,
	['H'] = 17, ['h'] = 17,
	['I'] = 18, ['i'] = 18,
	['J'] = 19, ['j'] = 19,
	['K'] = 20, ['k'] = 20,
	['L'] = 21, ['l'] = 21,
	['M'] = 22, ['m'] = 22,
	['N'] = 23, ['n'] = 23,
	['O'] = 24, ['o'] = 24,
	['P'] = 25, ['p'] = 25,
	['Q'] = 26, ['q'] = 26,
	['R'] = 27, ['r'] = 27,
	['S'] = 28, ['s'] = 28,
	['T'] = 29, ['t'] = 29,
	['U'] = 30, ['u'] = 30,
	['V'] = 31, ['v'] = 31,
	['W'] = 32, ['w'] = 32,
	['X'] = 33, ['x'] = 33,
	['Y'] = 34, ['y'] = 34,
	['Z'] = 35, ['z'] = 35,
};

Token* LexerInt(Lexer* lexer) {
	u64 length = 0;
	u64 value = 0;
	u64 base = 10;

	while (true) {
		switch (LexerPeek(lexer, length)) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '_':
			length++;
			continue;
		case '.':
			return LexerFloat(lexer);
		default:
			break;
		}
		length = 0;
		break;
	}

	if (lexer->Current == '0') {
		switch (LexerPeek(lexer, 1)) {
		case 'b': case 'B':
			base = 2;
			LexerNext(lexer);
			LexerNext(lexer);
			break;
		case 'x': case 'X':
			base = 16;
			LexerNext(lexer);
			LexerNext(lexer);
			break;
		default:
			base = 8;
			break;
		}
	}

	while (true) {
		switch (lexer->Current) {
		case '_':
			LexerNext(lexer);
			continue;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9': {
			u64 number = CharToNumber[(u8)lexer->Current];
			assert(
				number < base && // char not out of range
				(number != 0 || lexer->Current == '0') // invalid index
			); // @Incomplete: Implement actual error message

			value *= base;
			value += number;

			LexerNext(lexer);
		} continue;
		default:
			break;
		}
		break;
	}

	return CreateIntToken(value, length, lexer->Line, lexer->CharLinePosition, lexer->Source);
}

Token* LexerFloat(Lexer* lexer) {
	u64 length = 0;

	char* string = DynamicArrayCreate(char);

	while (true) {
		switch (lexer->Current) {
		case '_':
			LexerNext(lexer);
			continue;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '.': {
			DynamicArrayPush(string, lexer->Current);
			length++;
			LexerNext(lexer);
			continue;
		}
		default:
			break;
		}
		break;
	}

	DynamicArrayPushLiteral(string, '\0', char);

	char* end;
	double value = strtod(string, &end);

	DynamicArrayDestroy(string);

	return CreateFloatToken(value, length, lexer->Line, lexer->CharLinePosition, lexer->Source);
}
