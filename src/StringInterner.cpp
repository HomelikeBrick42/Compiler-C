#include "StringInterner.hpp"

#include <cstring>

struct InternedString {
	const char* String;
	uint64_t Length;
};

static InternedString* Strings = nullptr;
static uint64_t StringsCount = 0;

const char* InternString(const char* start, uint64_t length) {
	for (uint64_t i = 0; i < StringsCount; i++) {
		if (length == Strings[i].Length && strncmp(start, Strings[i].String, length) == 0) {
			return Strings[i].String;
		}
	}

	InternedString* newStrings = new InternedString[StringsCount + 1];
	memcpy(newStrings, Strings, StringsCount * sizeof(InternedString));
	char* string = new char[length + 1];
	memcpy(string, start, length);
	string[length] = '\0';
	newStrings[StringsCount++] = InternedString{ string, length };
	Strings = newStrings;
	return string;
}
