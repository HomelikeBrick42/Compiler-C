#include "NodeKind.hpp"

const char* NodeKindNames[(uint64_t)NodeKind::Count] = {
	"Error",
	"Root",
	"Int",
	"Float",
	"Identifier",
	"BinaryOperator",
	"UnaryOperator",
	"ConstantDefinition",
	"Definition",
	"DefinitionAssignment",
	"Assignment",
};

const char* NodeKindToString(NodeKind kind) {
    return NodeKindNames[(uint64_t)kind];
}
