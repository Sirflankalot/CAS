#include "cpp-cas.hpp"
#include "cas-impl.hpp"

namespace CAS {
	void _detail::impl_dtor::operator()(interpreter_impl* ptr) {
		delete ptr;
	}
}