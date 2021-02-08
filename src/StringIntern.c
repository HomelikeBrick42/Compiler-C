#include "StringIntern.h"
#include "Alloc.h"

#include <string.h>

typedef struct InternedString {
    const char* Text;
    u64 Length;
} InternedString;

static InternedString* Interns = nullptr;
static u64 InternCount = 0;

const char* StringIntern(const char* start, u64 length) {
	for (u64 i = 0; i < InternCount; i++) {
		if (Interns[i].Length == length && strncmp(Interns[i].Text, start, length) == 0) {
			return Interns[i].Text;
		}
	}

	char* newString = xcalloc(length + 1, sizeof(char));
	memcpy(newString, start, length);
	newString[length] = '\0';

	Interns = xrealloc(Interns, (InternCount + 1) * sizeof(InternedString));
	Interns[InternCount].Text = newString;
	Interns[InternCount].Length = length;
	InternCount++;

	return newString;
}
