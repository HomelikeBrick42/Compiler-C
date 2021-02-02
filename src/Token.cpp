#include "Token.hpp"
#include "TokenKind.hpp"

#include <cstdio>
	
void PrintToken(const char* source, Token* token) {
	switch (token->Kind) {
	case TokenKind::Int:
		printf("Token '%.*s' Type: <%s> Value: %lld\n", (int)token->Length, &source[token->Position], TokenKindToString(token->Kind), static_cast<IntToken*>(token)->Value);
		break;
	case TokenKind::Float:
		printf("Token '%.*s' Type: <%s> Value: %f\n", (int)token->Length, &source[token->Position], TokenKindToString(token->Kind), static_cast<FloatToken*>(token)->Value);
		break;
	default:
		printf("Token '%.*s' Type: <%s>\n", (int)token->Length, &source[token->Position], TokenKindToString(token->Kind));
		break;
	}
}
