#pragma once

#include <memory>

// Public interface for the interpreter/compiler.
// Uses the pimpl idiom to prevent implementation details
// from bleading into the API/ABI.

namespace CAS {
	// _detail namespace for all
	namespace _detail {
		// Implimentation class
		class interpreter_impl;

		// Function object that distructs an interpreter_impl
		struct impl_dtor {
			void operator()(interpreter_impl* ptr);
		};
	}

	class Interpreter {
	  public:
	  private:
		// Owning pointer to implimentation class, using the destructor object
		std::unique_ptr<_detail::interpreter_impl, _detail::impl_dtor> impl;
	};
}