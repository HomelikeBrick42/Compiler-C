#pragma once

#include <cstdint>

typedef enum DynamicArrayField
{
	DYNAMIC_ARRAY_CAPACITY,
	DYNAMIC_ARRAY_LENGTH,
	DYNAMIC_ARRAY_STRIDE,
	DYNAMIC_ARRAY_FIELD_LENGTH
} DynamicArrayField;

void* _DynamicArrayCreate(uint64_t length, uint64_t stride);
void _DynamicArrayDestroy(void* array);

uint64_t _DynamicArrayFieldGet(void* array, DynamicArrayField field);
void _DynamicArrayFieldSet(void* array, DynamicArrayField field, uint64_t value);

void* _DynamicArrayResize(void* array);

void* _DynamicArrayPush(void* array, void* value_ptr);
void _DynamicArrayPop(void* array, void* dest);

void* _DynamicArrayInsert(void* array, uint64_t index, void* value_ptr);
void _DynamicArrayPopAt(void* array, uint64_t index, void* dest);

#define DYNAMIC_ARAY_DEFAULT_CAPACITY 1
#define DYNAMIC_ARAY_RESIZE_FACTOR 2

#define DynamicArrayCreate(type) \
	(type*)_DynamicArrayCreate(DYNAMIC_ARAY_DEFAULT_CAPACITY, sizeof(type))

#define DynamicArrayReserve(type, capasity) \
	(type*)_DynamicArrayCreate(capasity, sizeof(type))

#define DynamicArrayDestroy(array) \
	_DynamicArrayDestroy(array); \
	array = nullptr

#define DynamicArrayPush(array, value) \
	array = (decltype(array))_DynamicArrayPush(array, &value)

#define DynamicArrayPushPtr(array, value) \
	array = (decltype(array))_DynamicArrayPush(array, value)

#define DynamicArrayPushLiteral(array, value) \
	{ \
		auto temp = value; \
		array = (decltype(array))_DynamicArrayPush(array, &temp); \
	}

#define DynamicArrayPop(array, value_ptr) \
	_DynamicArrayPop(array, value_ptr)

#define DynamicArrayInsert(array, index, value) \
	array = (decltype(array))_DynamicArrayInsert(array, index, &value)

#define DynamicArrayInsertPtr(array, index, value) \
	array = (decltype(array))_DynamicArrayInsert(array, index, value)

#define DynamicArrayInsertLiteral(array, index, value) \
	{ \
		auto temp = value; \
		array = (decltype(array))_DynamicArrayInsert(array, index, &temp); \
	}

#define DynamicArrayPopAt(array, index, value_ptr) \
	_DynamicArrayPopAt(array, index, value_ptr)

#define DynamicArrayGetCapacity(array) \
	_DynamicArrayFieldGet(array, DYNAMIC_ARRAY_CAPACITY)

#define DynamicArrayGetLength(array) \
	_DynamicArrayFieldGet(array, DYNAMIC_ARRAY_LENGTH)

#define DynamicArrayGetStride(array) \
	_DynamicArrayFieldGet(array, DYNAMIC_ARRAY_STRIDE)
