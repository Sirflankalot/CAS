#include "../cas-impl.hpp"

namespace CAS {
	namespace _detail {
		void interpreter_impl::eval_calculate(const char*) {}
		void interpreter_impl::eval_simplify(const char*) {}
		void interpreter_impl::eval_solve(const char*, std::vector<std::string>&) {}
		void interpreter_impl::eval_substitute(const char*) {}
		void interpreter_impl::eval_interpolate(const char*, Interpolation_t) {}
		void interpreter_impl::eval_root(const char*) {}
		void interpreter_impl::eval_limit(const char*) {}
		void interpreter_impl::eval_differentiate(const char*, Differentiation_t&) {}
		void interpreter_impl::eval_integrate(const char*) {}
		void interpreter_impl::eval_integrate_definite(const char*, Definite_Integration_t&) {}

		// Clear all state, as if starting the program new
		void interpreter_impl::clear_all_state() {
			this->variables.clear();
		}
	}
}