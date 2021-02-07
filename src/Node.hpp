#pragma once

#include <cstdint>

struct Token;

enum struct NodeKind : uint64_t;

struct Node {
	NodeKind Kind;
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
