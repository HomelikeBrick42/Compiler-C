#include "Token.hpp"
#include "TokenKind.hpp"
#include "Lexer.hpp"

#include <cstdio>
#include <cstdlib>
#include <assert.h>

int main(int argc, char** argv) {
	const char* text = "_a123 :: 1.5 + 0x45ff * 3 - 0b1000;";
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
