#include "NodeKind.hpp"

const char* NodeKindNames[(uint64_t)NodeKind::Count] = {
	"Error",
	"Int",
	"Float",
	"BinaryOperator",
	"UnaryOperator",
};

const char* NodeKindToString(NodeKind kind) {
    return NodeKindNames[(uint64_t)kind];
}
