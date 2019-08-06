#pragma once

#define PI 3.14159265359f

#undef min
#undef max

#if defined(__GNUC__) || defined(__clang__) || (defined(_MSC_VER) && _MSC_VER >= 1600)

#include <stdint.h>

#elif defined(_MSC_VER)
typedef signed   __int8		int8_t;
typedef unsigned __int8		uint8_t;
typedef signed   __int16	int16_t;
typedef unsigned __int16	uint16_t;
typedef signed   __int32	int32_t;
typedef unsigned __int32	uint32_t;
typedef signed   __int64	int64_t;
typedef unsigned __int64	uint64_t;
typedef uint64_t			uintptr_t;
typedef int64_t				intptr_t;
typedef int16_t				wchar_t;

#else

typedef signed   char		int8_t;
typedef unsigned char		uint8_t;
typedef signed   short int	int16_t;
typedef unsigned short int	uint16_t;
typedef signed   int		int32_t;
typedef unsigned int		uint32_t;
typedef long long			int64_t;
typedef unsigned long long	uint64_t;
typedef uint64_t			uintptr_t;
typedef int64_t				intptr_t;
typedef int16_t				wchar_t;

#endif

typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef int64_t		int64;
typedef uint8_t		CHART;
typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef uint64_t	uint64;
typedef intptr_t	intptr;
typedef uintptr_t	uintptr;
