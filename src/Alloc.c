#include "Alloc.h"

#include <stdlib.h>
#include <stdio.h>

void* xmalloc(u64 size) {
	void* ptr = malloc(size);
	if (!ptr) {
		perror("malloc failed!");
		exit(-1);
	}
	return ptr;
}

void* xcalloc(u64 count, u64 size) {
	void* ptr = calloc(count, size);
	if (!ptr) {
		perror("calloc failed!");
		exit(-1);
	}
	return ptr;
}

void* xrealloc(void* oldBlock, u64 size) {
	void* ptr = realloc(oldBlock, size);
	if (!ptr) {
		perror("realloc failed!");
		exit(-1);
	}
	return ptr;
}
