#pragma once

#include "Typedefs.h"

typedef enum TypeInfoKind {
	VOID,
	POINTER,
	INTEGER,
	FLOAT,
	PROCEDURE,
	STRUCT,
	ENUM,
} TypeInfoKind;

DECLARE_STRUCT(TypeInfo);
typedef struct TypeInfo {
	TypeInfoKind Kind;
	const char* TypeName;
	union {
		struct {
			u64 Size;
			bool Signed;
		} Integer;

		struct {
			u64 Size;
		} Float;

		struct {
			TypeInfo* PointerTo;
		} Pointer;

		struct {
			struct {
				const char* Name;
				TypeInfo* Type;
			} *Params;
			u64 ParamCount;
			TypeInfo* ReturnType;
		} Procedure;

		struct {
			struct {
				const char* Name;
				TypeInfo* Type;
			} *Members;
			u64 MemberCount;
		} Struct;

		struct {
			TypeInfo* Type;
			struct {
				const char* Name;
				u64 Value;
			} *Enum;
			u64 EnumCount;
		} Enum;
	};
} TypeInfo;
