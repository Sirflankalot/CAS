#include "cpp-cas.hpp"
#include "cas-impl.hpp"
#include "globals.hpp"
#include "util.hpp"

#include <cstdlib>
#include <iostream>

namespace CAS {
	// Destructor operator that knows the class's size
	void _detail::impl_dtor::operator()(interpreter_impl* ptr) {
		delete ptr;
	}

	// Constructor that knows the class's size
	Interpreter::Interpreter() {}

	// Clears all state inside the interpreter
	void Interpreter::clear_state() {
		impl->clear_all_state();
	}

	// Handles input and dispatches appropriate methods
	void Interpreter::input(const CAS::User_Input_t& input) {
		// Lambda to warn if arguments are present
		auto warn_on_args = [&]() {
			if (input.args.size()) {
				// Add warning to error manager
			}
		};

		switch (input.command) {
			case CAS::Command_t::NONE:
			case CAS::Command_t::CALCULATE:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::SIMPLIFY:
				warn_on_args();
				impl->eval_simplify(input.expression.c_str());
				break;

			// Parse arguments for Solve
			case CAS::Command_t::SOLVE: {
				std::vector<std::string> variables;
				size_t i = 0;

				auto&& a = input.args;

				// Skip whitespace
				_util::skip_whitespace(a, i);

				// check for "for "
				if (!(i + 4 < a.size())) {
					throw Create_Temp_Error("Expected \"for\"");
				}
				for (char c : "for ") {
					// If search is not in string, and search isn't null terminator
					if (a[i] != c) {
						if (c != '\0') {
							throw Create_Temp_Error("Expected \"for\"");
						}
						else {
							break;
						}
					}
					i += 1;
				}

				bool finished{false};

				// Collect variables
				while (!finished) {
					std::string var;

					_util::skip_whitespace(a, i);

					while (i < a.size() && !_util::is_whitespace(a[i])) {
						var.push_back(a[i]);
						i += 1;
					}

					_util::skip_whitespace(a, i);

					if (i + 1 >= a.size()) {
						finished = true;
					}
					else {
						_util::skip_whitespace(a, i);

						// Check for 'and '
						if (!(i + 4 < a.size())) {
							throw Create_Temp_Error("Expected \"and\"");
						}
						for (char c : "and ") {
							if (a[i] != c && c != '\0') {
								throw Create_Temp_Error("Expected \"and\"");
							}
							i += 1;
						}
					}

					variables.emplace_back(std::move(var));
				}

				impl->eval_solve(input.expression.c_str(), variables);
				break;
			}

			case CAS::Command_t::SUBSTITUTE:
				warn_on_args();
				impl->eval_substitute(input.expression.c_str());
				break;

			// Parse interpolations args
			case CAS::Command_t::INTERPOLATE: {
				_detail::Interpolation_t inter;

				if (input.args.size() == 0) {
					throw Create_Temp_Error("Arguments needed");
				}

				size_t i = 0;
				_util::skip_whitespace(input.args, i);

				auto a = input.args.substr(i);

				if (a == "constant") {
					inter = _detail::Interpolation_t::CONSTANT;
				}
				else if (a == "linear") {
					inter = _detail::Interpolation_t::LINEAR;
				}
				else if (a == "cosine") {
					inter = _detail::Interpolation_t::COSINE;
				}
				else if (a == "spline") {
					inter = _detail::Interpolation_t::SPLINE;
				}
				else {
					throw Create_Temp_Error("Invalid arguments");
				}

				impl->eval_interpolate(input.expression.c_str(), inter);
				break;
			}

			case CAS::Command_t::ROOT:
				warn_on_args();
				impl->eval_root(input.expression.c_str());
				break;

			case CAS::Command_t::LIMIT:
				warn_on_args();
				impl->eval_limit(input.expression.c_str());
				break;

			case CAS::Command_t::DIFFERENTIATE: {
				size_t i = 0;

				auto&& a = input.args;

				_detail::Differentiation_t diff{1, true};

				// Search for "partial "
				if (a.size() >= 8) {
					for (char c : "partial ") {
						// Reset if partial isn't found
						if (c != '\0') {
							if (a[i] != c) {
								diff.partial = false;
								i            = 0;
								break;
							}
						}
						else {
							break;
						}
						i += 1;
					}
				}
				else {
					diff.partial = false;
				}

				_util::skip_whitespace(a, i);

				// Search for n value
				if (_util::is_number(a[i])) {
					diff.n = atoll(a.c_str() + i);
				}
				else {
					throw Create_Temp_Error("Expected number");
				}

				impl->eval_differentiate(input.expression.c_str(), diff);
				break;
			}

			case CAS::Command_t::INTEGRATE:
				warn_on_args();
				impl->eval_integrate(input.expression.c_str());
				break;

			case CAS::Command_t::INTEGRATE_DEFINITE: {
				_detail::Definite_Integration_t def_int;

				auto&& a = input.args;

				size_t i = 0;

				_util::skip_whitespace(a, i);

				// Extract the variable name
				while (i < a.size() && a[i] != ' ') {
					def_int.variable.push_back(a[i]);
					i += 1;
				}
				i += 1;

				_util::skip_whitespace(a, i);

				// If there isn't a {, throw
				if (i >= a.size() || a[i] != '{') {
					throw Create_Temp_Error("Expected {");
				}
				i += 1;

				// Parse the first number after skipping whitespace
				_util::skip_whitespace(a, i);
				// Continue parsing until end of string or whitespace
				while (i < a.size() && !(a[i] == ',' || _util::is_whitespace(a[i]))) {
					if (!_util::is_numerical(a[i])) {
						throw Create_Temp_Error("Expected number");
					}
					def_int.start_num.push_back(a[i]);
					i += 1;
				}
				if (i + 1 >= a.size()) {
					throw Create_Temp_Error("Expected ,");
				}

				_util::skip_whitespace(a, i);

				if (a[i] == ',') {
					i += 1;
					_util::skip_whitespace(a, i);
				}

				while (i < a.size() && !(a[i] == '}' || _util::is_whitespace(a[i]))) {
					if (!_util::is_numerical(a[i])) {
						throw Create_Temp_Error("Expected number");
					}
					def_int.end_num.push_back(a[i]);
					i += 1;
				}
				if (i >= a.size()) {
					throw Create_Temp_Error("Expected }");
				}

				if (def_int.variable.size() == 0) {
					throw Create_Temp_Error("Expected variable name");
				}
				if (def_int.start_num.size() == 0) {
					throw Create_Temp_Error("Expected starting number");
				}
				if (def_int.end_num.size() == 0) {
					throw Create_Temp_Error("Expected ending number");
				}

				impl->eval_calculate(input.expression.c_str());
				break;
			}

			// Print help text
			case CAS::Command_t::HELP:
				*error_stream << "C++ CAS\n"
				              << CAS_COPYRIGHT "\n"
				              << "version " CAS_VERSION_STR "\n"
				              << CAS_COMMAND_HELP_TEXT;

			// Get the compiler to shut up
			case CAS::Command_t::QUIT:
				break;
		}
	}
}
