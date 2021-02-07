#include "Parser.hpp"
#include "Node.hpp"
#include "NodeKind.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>

int main(int argc, char** argv) {
	const char* text =
		"_var123 := 1 + 2 * 3;\n"
		"a :: 10;\n"
		"b :: 20;\n"
		"c := a + b;\n"
		"c = a * b + c;\n"
		"c *= a;\n";
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
