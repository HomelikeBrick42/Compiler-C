#include "Node.hpp"
#include "NodeKind.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"
#include "DynamicArray.hpp"

#include <cstdio>
#include <cstring>
#include <cassert>

void PrintNode(Node* node, const char* indent, bool isLast) {
	const char* marker = isLast ? "\xC0\xC4\xC4" : "\xC3\xC4\xC4";

    printf("%s%s", indent, marker);
    printf("%s", NodeKindToString(node->Kind));

    Token* token = nullptr;
    switch (node->Kind) {
    case NodeKind::Identifier:
        printf(" %s", static_cast<IdentifierToken*>(static_cast<IdentifierNode*>(node)->Identifier)->Name);
        break;
    case NodeKind::ConstantDefinition:
        if (static_cast<ConstantDefinitionNode*>(node)->Type) {
            assert(false);
        }
        printf(" %s", static_cast<IdentifierToken*>(static_cast<ConstantDefinitionNode*>(node)->Identifier)->Name);
        break;
    case NodeKind::Definition:
        if (static_cast<DefinitionNode*>(node)->Type) {
            assert(false);
        }
        printf(" %s", static_cast<IdentifierToken*>(static_cast<DefinitionNode*>(node)->Identifier)->Name);
        break;
    case NodeKind::Assignment:
        printf(" %s", static_cast<IdentifierToken*>(static_cast<AssignmentNode*>(node)->Identifier)->Name);
        token = static_cast<AssignmentNode*>(node)->Operator;
        break;
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
        case TokenKind::PlusEquals:
            printf("+=");
            break;
        case TokenKind::MinusEquals:
            printf("-=");
            break;
        case TokenKind::AsteriskEquals:
            printf("*=");
            break;
        case TokenKind::ForwardSlashEquals:
            printf("/=");
            break;
        case TokenKind::PercentEquals:
            printf("%=");
            break;
        case TokenKind::LessThanLessThanEquals:
            printf("<<=");
            break;
        case TokenKind::GreaterThanGreaterThanEquals:
            printf(">>=");
            break;
        case TokenKind::AmpersandEquals:
            printf("&=");
            break;
        case TokenKind::PipeEquals:
            printf("|=");
            break;
        case TokenKind::CaretEquals:
            printf("^=");
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
    case NodeKind::Root:
        for (uint64_t i = 0; i < DynamicArrayGetLength(static_cast<RootNode*>(node)->Children); i++) {
            PrintNode(static_cast<RootNode*>(node)->Children[i], newIndent, i == (DynamicArrayGetLength(static_cast<RootNode*>(node)->Children) - 1));
        }
        break;
    case NodeKind::UnaryOperator:
        PrintNode(static_cast<UnaryOperatorNode*>(node)->Operand, newIndent, true);
        break;
    case NodeKind::BinaryOperator:
        PrintNode(static_cast<BinaryOperatorNode*>(node)->Left, newIndent, false);
        PrintNode(static_cast<BinaryOperatorNode*>(node)->Right, newIndent, true);
        break;
    case NodeKind::ConstantDefinition:
        PrintNode(static_cast<ConstantDefinitionNode*>(node)->Value, newIndent, true);
        break;
    case NodeKind::DefinitionAssignment:
        PrintNode(static_cast<DefinitionAssignmentNode*>(node)->Value, newIndent, true);
        break;
    case NodeKind::Assignment:
        PrintNode(static_cast<AssignmentNode*>(node)->Value, newIndent, true);
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
