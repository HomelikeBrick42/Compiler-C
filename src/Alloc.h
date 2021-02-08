#pragma once

#include "Typedefs.h"

void* xmalloc(u64 size);
void* xcalloc(u64 count, u64 size);
void* xrealloc(void* oldBlock, u64 size);
