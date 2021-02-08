#include "Lexer.h"
#include "TokenKind.h"
#include "Token.h"
#include "Alloc.h"
#include "StringIntern.h"

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
    default: return CreateToken(LexerNext(lexer), lexer->Line, lexer->CharLinePosition, lexer->Source);
    }
}

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
	while (true) {
		switch (lexer->Current) {
		case ' ': case '\n': case '\r':
		case '\t': case '\v': case '\f':
			LexerNext(lexer);
			continue;
		default:
			break;
		}
		break;
	}

	return CreateToken(TOKEN_KIND_WHITESPACE, lexer->Line, lexer->CharLinePosition, lexer->Source);
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

	return CreateIdentifierToken(StringIntern(&lexer->Source[start], length), lexer->Line, lexer->CharLinePosition, lexer->Source);
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

	return CreateIntToken(value, lexer->Line, lexer->CharLinePosition, lexer->Source);
}

Token* LexerFloat(Lexer* lexer) {
	u64 length = 0;

	char* string = xcalloc(1, sizeof(char));

	while (true) {
		switch (lexer->Current) {
		case '_':
			LexerNext(lexer);
			continue;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '.': {
			string = xrealloc(string, (length + 2) * sizeof(char));
			string[length + 0] = lexer->Current;
			string[length + 1] = '\0';
			length++;
			LexerNext(lexer);
			continue;
		}
		default:
			break;
		}
		break;
	}

	char* end;
	double value = strtod(string, &end);
	free(string);

	return CreateFloatToken(value, lexer->Line, lexer->CharLinePosition, lexer->Source);
}
