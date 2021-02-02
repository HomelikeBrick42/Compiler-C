#include "Token.hpp"
#include "TokenKind.hpp"
#include "Lexer.hpp"

#include <cstdio>

int main(int argc, char** argv) {
	const char* text = "_a123 :: 1.5e-1 + 2 * 3;";
	Lexer lexer(text);
	while (true) {
		Token* token = lexer.NextToken();
		PrintToken(text, token);
		if (token->Kind == TokenKind::EndOfFile)
			break;
	}
	(void)getchar();
	return 0;
}
