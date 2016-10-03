#include "cas-impl.hpp"

#include <array>
#include <iostream>

namespace CAS {
	namespace _detail {
		void interpreter_impl::eval_calculate(const std::string& code, size_t line_num,
		                                      size_t char_num) {
			auto tokens = tokenize(code, line_num, char_num);
			print_tokens(tokens);
		}
		void interpreter_impl::eval_simplify(const std::string&, size_t, size_t) {}
		void interpreter_impl::eval_solve(const std::string&, std::vector<std::string>&, size_t,
		                                  size_t) {}
		void interpreter_impl::eval_substitute(const std::string&, size_t, size_t) {}
		void interpreter_impl::eval_interpolate(const std::string&, Interpolation_t, size_t,
		                                        size_t) {}
		void interpreter_impl::eval_root(const std::string&, size_t, size_t) {}
		void interpreter_impl::eval_limit(const std::string&, size_t, size_t) {}
		void interpreter_impl::eval_differentiate(const std::string&, Differentiation_t&, size_t,
		                                          size_t) {}
		void interpreter_impl::eval_integrate(const std::string&, size_t, size_t) {}
		void interpreter_impl::eval_integrate_definite(const std::string&, Definite_Integration_t&,
		                                               size_t, size_t) {}

		// Clear all state, as if starting the program new
		void interpreter_impl::clear_all_state() {
			this->variables.clear();
		}
	}
}