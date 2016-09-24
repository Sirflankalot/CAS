#include "cpp-cas.hpp"
#include "cas-impl.hpp"

namespace CAS {
	Interpreter::Interpreter() {}

	void _detail::impl_dtor::operator()(interpreter_impl* ptr) {
		delete ptr;
	}
}