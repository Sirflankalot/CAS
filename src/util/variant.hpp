#pragma once

namespace CAS {
	namespace _util {
		template <class T1, class... T>
		class Heap_Variant;

		template <class T1, class... T>
		class Stack_Variant;

		template <class T1, class... T>
		using Variant = Stack_Variant<T1, T...>;

		struct bad_variant_get;
	}
}