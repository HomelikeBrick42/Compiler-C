#pragma once

#include "Typedefs.h"

typedef enum TypeInfoKind {
	TYPE_INFO_VOID,
	TYPE_INFO_POINTER,
	TYPE_INFO_INTEGER,
	TYPE_INFO_FLOAT,
	TYPE_INFO_ARRAY,
	TYPE_INFO_STRING,
	TYPE_INFO_PROCEDURE,
	TYPE_INFO_STRUCT,
	TYPE_INFO_ENUM,
} TypeInfoKind;

DECLARE_STRUCT(TypeInfo);
typedef struct TypeInfo {
	TypeInfoKind Kind;
	const char* TypeName;
	union {
		struct {
			TypeInfo* ToType;
		} Pointer;

		struct {
			u64 Size;
			bool Signed;
		} Integer;

		struct {
			u64 Size;
		} Float;

		struct {
			TypeInfo* OfType;
		} Array;

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
