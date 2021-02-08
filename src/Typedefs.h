#pragma once

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed long long	s64;

typedef float				f32;
typedef double				f64;

#if !defined(_MSC_VER)
	typedef long double			f80;
#endif

#if defined(__cplusplus)
	#define SIZE_ASSERT(type, size) \
		static_assert(sizeof(type) == size, "Expected sizeof " #type " to be " #size " bytes.")
#else
	#define SIZE_ASSERT(type, size) \
		_Static_assert(sizeof(type) == size, "Expected sizeof " #type " to be " #size " bytes.")
#endif

SIZE_ASSERT(u8, 1);
SIZE_ASSERT(u16, 2);
SIZE_ASSERT(u32, 4);
SIZE_ASSERT(u64, 8);

SIZE_ASSERT(s8, 1);
SIZE_ASSERT(s16, 2);
SIZE_ASSERT(s32, 4);
SIZE_ASSERT(s64, 8);

SIZE_ASSERT(f32, 4);
SIZE_ASSERT(f64, 8);

#if !defined(_MSC_VER)
	SIZE_ASSERT(f80, 10);
#endif

#undef SIZE_ASSERT

#if !defined(__cplusplus)
	#define bool _Bool
	enum {
		false = 0,
		true = 1
	};

	#define nullptr ((void*)0)
#endif

#define DECLARE_STRUCT(name) typedef struct name name
#define DECLARE_ENUM(name) typedef enum name name
