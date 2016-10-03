#include "cas-impl.hpp"

#include <array>
#include <iostream>

namespace CAS {
	namespace _detail {
		void interpreter_impl::eval_calculate(const std::string& code) {
			auto tokens = tokenize(code);
			print_tokens(tokens);
		}
		void interpreter_impl::eval_simplify(const std::string&) {}
		void interpreter_impl::eval_solve(const std::string&, std::vector<std::string>&) {}
		void interpreter_impl::eval_substitute(const std::string&) {}
		void interpreter_impl::eval_interpolate(const std::string&, Interpolation_t) {}
		void interpreter_impl::eval_root(const std::string&) {}
		void interpreter_impl::eval_limit(const std::string&) {}
		void interpreter_impl::eval_differentiate(const std::string&, Differentiation_t&) {}
		void interpreter_impl::eval_integrate(const std::string&) {}
		void interpreter_impl::eval_integrate_definite(const std::string&,
		                                               Definite_Integration_t&) {}

		// Clear all state, as if starting the program new
		void interpreter_impl::clear_all_state() {
			this->variables.clear();
		}
	}
}