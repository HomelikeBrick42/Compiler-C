#pragma once

#include <cstdint>

enum struct NodeKind : uint64_t {
	Error,
	Root,
	Int,
	Float,
	Identifier,
	BinaryOperator,
	UnaryOperator,
	ConstantDefinition,
	Definition,
	DefinitionAssignment,
	Assignment,

	Count,
};

const char* NodeKindToString(NodeKind kind);
