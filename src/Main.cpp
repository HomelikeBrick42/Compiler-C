#include "Parser.hpp"
#include "Node.hpp"
#include "NodeKind.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>

int main(int argc, char** argv) {
	const char* text = "(1.5 + -0x45ff * 3 - 0b1000 ^ 5 | 0x00ff) << 2;";
/*
	Lexer lexer(text);

	while (true) {
		Token* token = lexer.NextToken();
		PrintToken(text, token);
		if (token->Kind == TokenKind::EndOfFile) {
			break;
		}
	}
*/
	Parser parser(text);
	Node* root = parser.Parse();
	PrintNode(root);

	(void)getchar();
	return 0;
}
