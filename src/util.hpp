#pragma once

#include <cstdlib>
#include <stdexcept>

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

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define Create_Temp_Error(str) CAS::_util::Temp_Error(str " at " __FILE__ ":" TOSTRING(__LINE__))

namespace CAS {
	namespace _util {
		// A utility function to check if a char is in a list of
		// other characters
		template <size_t loc>
		ALWAYS_INLINE size_t impl_char_is_in(char cmp) {
			(void) cmp;
			return 0;
		}

		template <size_t loc, class... Args>
		ALWAYS_INLINE size_t impl_char_is_in(char cmp, char c1, Args... a) {
			if (cmp == c1) {
				return loc;
			}
			else {
				return impl_char_is_in<loc + 1>(cmp, a...);
			}
		}

		template <class... Args>
		inline size_t char_is_in(char cmp, Args... a) {
			return impl_char_is_in<1>(cmp, a...);
		}

		// Related utilites for specialties.
		inline bool is_whitespace(char cmp) {
			return char_is_in(cmp, ' ', '\t');
		}

		// Basic error thing
		class Temp_Error : public virtual std::exception {
		  private:
			const char* string;

		  public:
			Temp_Error(const char* str) : string(str) {}
			virtual const char* what() const noexcept override {
				return string;
			}
		};
	}
}
