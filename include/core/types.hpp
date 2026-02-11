#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <cstdint>
#include <cstddef>

// types
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using ssize = ptrdiff_t;

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

using bool32 = s32;

using byte = u8;

// string
struct WString {
	wchar* wstring;
	s64 count;
};

// utility
#define ARRAY_COUNT(arr) (sizeof((arr)) / sizeof((arr)[0]))

// debugging
#if !defined(NDEBUG)
#	define ASSERT(x)     \
		if(!(x)){         \
			*(int*)0 = 0;  \
		}
#else
#	define ASSERT(x)
#endif // !defined(NDEBUG)

#endif // CORE_TYPES_HPP
