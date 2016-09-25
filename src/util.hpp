#pragma once

#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <type_traits>

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
		// A utility struct to deduce function parameters
		// primary template. I tried to write this myself,
		// but I kept failing, so I copy pasted it from
		// stack overflow. Template metaprogramming is difficult
		// sometimes :/ I tried to get this to work for too damn
		// long, something on the order of 2 hours...
		template <class T>
		struct function_traits : function_traits<decltype(&T::operator())> {};

		// partial specialization for function type
		template <class R, class... Args>
		struct function_traits<R(Args...)> {
			using result_type    = R;
			using argument_types = std::tuple<Args...>;
		};

		// partial specialization for function pointer
		template <class R, class... Args>
		struct function_traits<R (*)(Args...)> {
			using result_type    = R;
			using argument_types = std::tuple<Args...>;
		};

		// partial specialization for std::function
		template <class R, class... Args>
		struct function_traits<std::function<R(Args...)>> {
			using result_type    = R;
			using argument_types = std::tuple<Args...>;
		};

		// partial specialization for pointer-to-member-function (i.e., operator()'s)
		template <class T, class R, class... Args>
		struct function_traits<R (T::*)(Args...)> {
			using result_type    = R;
			using argument_types = std::tuple<Args...>;
		};

		template <class T, class R, class... Args>
		struct function_traits<R (T::*)(Args...) const> {
			using result_type    = R;
			using argument_types = std::tuple<Args...>;
		};

		// I did write everything else by hand though
		template <size_t n, class Func>
		struct function_arg {
			using type =
			    typename std::tuple_element<n,
			                                typename function_traits<Func>::argument_types>::type;
		};

		// A utility function to check if a char is in a list of
		// other characters
		template <size_t loc>
		constexpr ALWAYS_INLINE size_t _impl_char_is_in(char cmp) {
			(void) cmp;
			return 0;
		}

		template <size_t loc, class... Args>
		constexpr ALWAYS_INLINE size_t _impl_char_is_in(char cmp, char c1, Args... a) {
			if (cmp == c1) {
				return loc;
			}
			else {
				return _impl_char_is_in<loc + 1>(cmp, a...);
			}
		}

		template <class... Args>
		constexpr inline size_t char_is_in(char cmp, Args... a) {
			return _impl_char_is_in<1>(cmp, a...);
		}

		// Related utilites for specialties.
		constexpr inline bool is_whitespace(char cmp) {
			return char_is_in(cmp, ' ', '\t');
		}

		constexpr inline bool is_number(char cmp) {
			return ('0' <= cmp) && (cmp <= '9');
		}

		constexpr inline bool is_numerical(char cmp) {
			return is_number(cmp) || char_is_in(cmp, '.', '-', '+');
		}

		constexpr inline bool is_letter(char cmp) {
			return (('A' <= cmp) && (cmp <= 'Z')) || (('a' <= cmp) && (cmp <= 'z'));
		}

		// Takes a container, an index into the container, and a unary functor to automatically
		// iterate the index until the functor decides that the current location isn't skippable
		// (or there isn't anything left in the string)
		template <class Container, class Func>
		constexpr inline void skip_elements(Container&& c, size_t& i, Func unary) {
			// All that metaprogramming for this one static assert
			static_assert(
			    std::is_convertible<decltype(c[i]), typename function_arg<0, Func>::type>::value,
			    "Unary function doesn't accept correct argument");

			while (i < c.size() && unary(c[i])) {
				i += 1;
			}
		}

		template <class Container>
		constexpr inline void skip_whitespace(Container&& c, size_t& i) {
			skip_elements(c, i, is_whitespace);
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
