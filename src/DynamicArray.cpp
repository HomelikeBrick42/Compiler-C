#include "DynamicArray.hpp"

#include <cstdlib>
#include <cstdio>
#include <memory.h>

void* _DynamicArrayCreate(uint64_t length, uint64_t stride) {
	uint64_t headerSize = DYNAMIC_ARRAY_FIELD_LENGTH * sizeof(uint64_t);
	uint64_t arraySize = length * stride;
	uint64_t* newArray = (uint64_t*)malloc(headerSize + arraySize);
	newArray[DYNAMIC_ARRAY_CAPACITY] = length;
	newArray[DYNAMIC_ARRAY_LENGTH] = 0;
	newArray[DYNAMIC_ARRAY_STRIDE] = stride;
	return newArray + DYNAMIC_ARRAY_FIELD_LENGTH;
}

void _DynamicArrayDestroy(void* array) {
	free((uint64_t*)array - DYNAMIC_ARRAY_FIELD_LENGTH);
}

uint64_t _DynamicArrayFieldGet(void* array, DynamicArrayField field) {
	uint64_t* header = (uint64_t*)array - DYNAMIC_ARRAY_FIELD_LENGTH;
	return header[field];
}

void _DynamicArrayFieldSet(void* array, DynamicArrayField field, uint64_t value) {
	uint64_t* header = (uint64_t*)array - DYNAMIC_ARRAY_FIELD_LENGTH;
	header[field] = value;
}

void* _DynamicArrayResize(void* array) {
	uint64_t length = DynamicArrayGetLength(array);
	uint64_t stride = DynamicArrayGetStride(array);
	void* newArray = _DynamicArrayCreate((uint64_t)(DYNAMIC_ARAY_RESIZE_FACTOR * DynamicArrayGetCapacity(array)), stride);
	memcpy(newArray, array, length * stride);
	_DynamicArrayFieldSet(newArray, DYNAMIC_ARRAY_LENGTH, length);
	_DynamicArrayDestroy(array);
	return newArray;
}

void* _DynamicArrayPush(void* array, void* value_ptr) {
	uint64_t length = DynamicArrayGetLength(array);
	uint64_t stride = DynamicArrayGetStride(array);
	if (length >= DynamicArrayGetCapacity(array)) {
		array = _DynamicArrayResize(array);
	}
	uint64_t addr = (uint64_t)array;
	addr += length * stride;
	memcpy((void*)addr, value_ptr, stride);
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length + 1);
	return array;
}

void _DynamicArrayPop(void* array, void* dest) {
	uint64_t length = DynamicArrayGetLength(array);
	uint64_t stride = DynamicArrayGetStride(array);
	uint64_t addr = (uint64_t)array;
	addr += (length - 1) * stride;
	memcpy(dest, (void*)addr, stride);
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length - 1);
}

void* _DynamicArrayInsert(void* array, uint64_t index, void* value_ptr) {
	uint64_t length = DynamicArrayGetLength(array);
	uint64_t stride = DynamicArrayGetStride(array);
	if (length >= DynamicArrayGetCapacity(array)) {
		array = _DynamicArrayResize(array);
	}
	uint64_t addr = (uint64_t)array;
	if (index != length - 1) {
		memcpy(
			(void*)(addr + (index + 1) * stride),
			(void*)(addr + index * stride),
			stride * (length - index)
		);
	}
	memcpy((void*)(addr + index * stride), value_ptr, stride);
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length + 1);
	return array;
}

void _DynamicArrayPopAt(void* array, uint64_t index, void* dest) {
	uint64_t length = DynamicArrayGetLength(array);
	uint64_t stride = DynamicArrayGetStride(array);
	if (length >= DynamicArrayGetCapacity(array)) {
		printf("Index outside the bounds of this array! Length: %llu, Index: %llu", length, index);
		return;
	}
	uint64_t addr = (uint64_t)array;
	if (dest) {
		memcpy(dest, (void*)(addr + index * stride), stride);
	}
	if (index != length - 1) {
		memcpy(
			(void*)(addr + index * stride),
			(void*)(addr + (index + 1) * stride),
			stride * (length - index)
		);
	}
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length - 1);
}
