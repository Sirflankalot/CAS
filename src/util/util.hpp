#pragma once

#include <cstdlib>

// clang-format off
// Multi-platform non-debug force inlining
#ifndef DEBUG
	#ifdef _MSC_VER
		#define ALWAYS_INLINE __forceinline
	#elif defined __GNUC__
		#define ALWAYS_INLINE __attribute__((always_inline)) inline
	#else
		#define ALWAYS_INLINE inline
	#endif
#else
	#define ALWAYS_INLINE inline
#endif
// clang-format on

// Temporary error system
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define Create_Temp_Error(str) CAS::_util::Temp_Error(str " at " __FILE__ ":" TOSTRING(__LINE__))

namespace CAS {
	namespace _util {
		// Constexpr in case all arguments are also constexpr

		// Tells if the char cmp is in the list of chars following it (so `char_is_in(x, ',', '.')`
		// will be true if x is either a comma or a period)
		template <class... Args>
		constexpr inline size_t char_is_in(char cmp, Args... a);

		constexpr inline bool is_whitespace(char cmp); // if cmp is whitespace
		constexpr inline bool is_number(char cmp);     // if cmp is number
		constexpr inline bool is_numerical(char cmp);  // if cmp is a number or a '-', '+', or '.'
		constexpr inline bool is_letter(char cmp);     // if cmp is a regular letter
		constexpr inline bool is_unprefixed_numerical(char cmp); // if cmp is a number or '.'.

		// Takes a container, an index into the container, and a unary functor. It will
		// automatically iterate the index until the functor decides that the current location isn't
		// skippable (or there isn't anything left in the container)
		template <class Container, class Func>
		constexpr inline void skip_elements(Container&& c, size_t& i, Func unary);

		// Takes a container (std::string or std::vector<char>), and an index into that container,
		// and iterates the index utill the character pointed at by the index is not whitespace, or
		// the string has ran out of characters
		template <class Container>
		constexpr inline void skip_whitespace(Container&& c, size_t& i);

		// Basic error thing
		class Temp_Error;
	}
}

#include "util.tpp"
