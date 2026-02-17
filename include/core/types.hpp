#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <cstdint>
#include <cstddef>

// types
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using uint = unsigned int;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = size_t;

using f32 = float;
using f64 = double;

using uchar = unsigned char;
using wchar = wchar_t;

using bool32 = i32;

// memory types
using byte = u8;
using uptr = uintptr_t;	// unsigned memory management
using iptr = intptr_t;	// signed memory management 
using ptroffset = ptrdiff_t;

// utility
#define array_count(arr) (sizeof((arr)) / sizeof((arr)[0]))

// debugging
#if !defined(NDEBUG)
#	define assert(x)     \
		if(!(x)){         \
			*(int*)0 = 0;  \
		}
#else
#	define assert(x)
#endif // !defined(NDEBUG)

#endif // CORE_TYPES_HPP
