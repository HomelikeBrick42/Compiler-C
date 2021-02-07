#pragma once

#include <cstdint>

enum struct NodeKind : uint64_t {
	Error,
	Int,
	Float,
	BinaryOperator,
	UnaryOperator,

	Count,
};

const char* NodeKindToString(NodeKind kind);
