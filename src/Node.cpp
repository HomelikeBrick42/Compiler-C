#include "Node.hpp"
#include "NodeKind.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"

#include <cstdio>
#include <cstring>

void PrintNode(Node* node, const char* indent, bool isLast) {
	const char* marker = isLast ? "\xC0\xC4\xC4" : "\xC3\xC4\xC4";

    printf(indent);
    printf(marker);
    printf("%s", NodeKindToString(node->Kind));
    Token* token = nullptr;
    switch (node->Kind) {
    case NodeKind::UnaryOperator:
        token = static_cast<UnaryOperatorNode*>(node)->Operator;
        break;
    case NodeKind::BinaryOperator:
        token = static_cast<BinaryOperatorNode*>(node)->Operator;
        break;
    }

    if (token) {
        printf(" ");
        switch (token->Kind)
        {
        case TokenKind::ExclamationMark:
			printf("!");
			break;
        case TokenKind::LessThanLessThan:
			printf("<<");
			break;
        case TokenKind::GreaterThanGreaterThan:
			printf(">>");
			break;
        case TokenKind::Caret:
			printf("^");
			break;
        case TokenKind::Ampersand:
			printf("&");
			break;
        case TokenKind::Asterisk:
			printf("*");
			break;
        case TokenKind::ForwardSlash:
			printf("/");
			break;
        case TokenKind::Percent:
			printf("%%");
			break;
        case TokenKind::Pipe:
			printf("|");
			break;
        case TokenKind::Plus:
			printf("+");
			break;
        case TokenKind::Minus:
			printf("-");
			break;
        case TokenKind::LessThan:
			printf("<");
			break;
        case TokenKind::GreaterThan:
			printf(">");
			break;
        case TokenKind::LessThanEquals:
			printf("<=");
			break;
        case TokenKind::GreaterThanEquals:
			printf(">=");
			break;
        case TokenKind::EqualsEquals:
			printf("==");
			break;
        case TokenKind::ExclamationMarkEquals:
			printf("!=");
			break;
        case TokenKind::AmpersandAmpersand:
			printf("&&");
			break;
        case TokenKind::PipePipe:
            printf("||");
            break;
        }
    }

    switch (node->Kind) {
    case NodeKind::Int: {
        printf(" %lld", static_cast<IntNode*>(node)->Value);
    } break;
    case NodeKind::Float: {
        printf(" %f", static_cast<FloatNode*>(node)->Value);
    } break;
    default:
        break;
    }

    printf("\n");

    char* newIndent = new char[strlen(indent) + 4];
    strcpy(newIndent, indent);
    strcat(newIndent, isLast ? "   " : "\xB3  ");

    switch (node->Kind) {
    case NodeKind::UnaryOperator:
        PrintNode(static_cast<UnaryOperatorNode*>(node)->Operand, newIndent, true);
        break;
    case NodeKind::BinaryOperator:
        PrintNode(static_cast<BinaryOperatorNode*>(node)->Left, newIndent, false);
        PrintNode(static_cast<BinaryOperatorNode*>(node)->Right, newIndent, true);
        break;
    case NodeKind::Int:
    case NodeKind::Float:
        break;
    }

    delete[] newIndent;
}

void PrintNode(Node* root) {
	PrintNode(root, "", true);
}
