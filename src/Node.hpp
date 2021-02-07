#pragma once

#include <cstdint>

struct Token;
struct TypeInfo;

enum struct NodeKind : uint64_t;

struct Node {
	NodeKind Kind;
};

struct RootNode : Node {
	Node** Children;
};

struct DefinitionNode : Node {
	Token* Identifier;
	TypeInfo* Type;
};

struct DefinitionAssignmentNode : DefinitionNode {
	Node* Value;
};

struct ConstantDefinitionNode : DefinitionAssignmentNode {
};

struct IdentifierNode : Node {
	Token* Identifier;
};

struct AssignmentNode : Node {
	Token* Identifier;
	Token* Operator;
	Node* Value;
};

struct BinaryOperatorNode : Node {
	Node* Left;
	Token* Operator;
	Node* Right;
};

struct UnaryOperatorNode : Node {
	Token* Operator;
	Node* Operand;
};

struct IntNode : Node {
	uint64_t Value;
};

struct FloatNode : Node {
	double Value;
};

void PrintNode(Node* root);
