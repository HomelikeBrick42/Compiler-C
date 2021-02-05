#pragma once

#include <cstdint>

enum struct TokenKind {
	// Single char
	EndOfFile,
	Bad,
	OpenParentheses,
	CloseParentheses,
	OpenSquareBracket,
	CloseSquareBracket,
	OpenBracket,
	CloseBracket,
	Semicolon,
	QuestionMark,
	Comma,
	Grave,
	Dot,
	Plus,
	Minus,
	Asterisk,
	ForwardSlash,
	Percent,
	Caret,
	Colon,
	Equals,
	ExclamationMark,
	Ampersand,
	Pipe,
	LessThan,
	GreaterThan,

	// Double char
	PlusEquals,
	MinusEquals,
	RightArrow,
	AsteriskEquals,
	ForwardSlashEquals,
	PercentEquals,
	CaretEquals,
	ColonColon,
	ColonEquals,
	EqualsEquals,
	ExclamationMarkEquals,
	AmpersandAmpersand,
	AmpersandEquals,
	PipePipe,
	PipeEquals,
	LessThanLessThan,
	GreaterThanGreaterThan,
	LessThanEquals,
	GreaterThanEquals,
	
	// Triple char
	LessThanLessThanEquals,
	GreaterThanGreaterThanEquals,

	// Multichar
	Whitespace,
	Comment,
	Identifier,
	Int,
	Float,

	Count,
};

extern const char* TokenKindNames[(uint64_t)TokenKind::Count];
inline const char* TokenKindToString(TokenKind kind) {
	return TokenKindNames[(uint64_t)kind];
}
