#include "File.h"
#include "Alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char* LoadFile(const char* filepath) {
    char* buffer = nullptr;
    u64 stringSize = 0, readSize = 0;
    FILE* fileHandle = fopen(filepath, "r");

    if (fileHandle) {
        fseek(fileHandle, 0, SEEK_END);
        stringSize = ftell(fileHandle);
        rewind(fileHandle);

        buffer = (char*)xcalloc(stringSize + 1, sizeof(char));
        assert(buffer != nullptr);
        readSize = fread(buffer, sizeof(char), stringSize, fileHandle);

        fclose(fileHandle);
    }

    return buffer;
}
