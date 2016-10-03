#include "cpp-cas.hpp"
#include "cas-impl.hpp"
#include "globals.hpp"
#include "util/util.hpp"

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

		// Lambda that throws an error if command that needs
		// arguments isn't supplied any arguments
		auto error_with_no_args = [&]() {
			if (input.args.size() == 0) {
				throw Create_Temp_Error("Arguments needed");
			}
		};

		switch (input.command) {
			// Forwards to default behavior
			case CAS::Command_t::NONE:
			case CAS::Command_t::CALCULATE:
				warn_on_args();
				impl->eval_calculate(input.expression, input.line, input.char_expression);
				break;

			// Forwards to simplification method
			case CAS::Command_t::SIMPLIFY:
				warn_on_args();
				impl->eval_simplify(input.expression, input.line, input.char_expression);
				break;

			// Parse arguments for solve method
			// then call solve
			case CAS::Command_t::SOLVE: {
				error_with_no_args();

				size_t i = 0;
				auto&& a = input.args;

				std::vector<std::string> variables;

				_util::skip_whitespace(a, i);

				// check for a "for" at the current location
				if (!(i + 3 < a.size())) {
					throw Create_Temp_Error("Expected \"for\"");
				}
				for (char c : "for") {
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

				// Collect all variable names one per loop
				while (!finished) {
					std::string var;

					_util::skip_whitespace(a, i);

					// Grab first name
					while (i < a.size() && !_util::is_whitespace(a[i])) {
						var.push_back(a[i]);
						i += 1;
					}

					_util::skip_whitespace(a, i);

					// If we are at the end of the string, mark finished
					if (i + 1 >= a.size()) {
						finished = true;
					}
					else {
						_util::skip_whitespace(a, i);

						// Check for 'and'
						if (!(i + 3 < a.size())) {
							throw Create_Temp_Error("Expected \"and\"");
						}
						for (char c : "and") {
							if (a[i] != c && c != '\0') {
								throw Create_Temp_Error("Expected \"and\"");
							}
							i += 1;
						}
					}

					// Move construct the variable name onto the list
					variables.emplace_back(std::move(var));
				}

				// Call solve function
				impl->eval_solve(input.expression, variables, input.line, input.char_expression);
				break;
			}

			// Forwards to substitution method
			case CAS::Command_t::SUBSTITUTE:
				warn_on_args();
				impl->eval_substitute(input.expression, input.line, input.char_expression);
				break;

			// Parse interpolations args
			case CAS::Command_t::INTERPOLATE: {
				error_with_no_args();

				// Compare with a string that has the whitespace cut off
				size_t i = 0;
				_util::skip_whitespace(input.args, i);
				auto a = input.args.substr(i);

				_detail::Interpolation_t inter;

				// Check for each argument
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

				// Call interpolate function
				impl->eval_interpolate(input.expression, inter, input.line, input.char_expression);
				break;
			}

			// Forwards to rooting method
			case CAS::Command_t::ROOT:
				warn_on_args();
				impl->eval_root(input.expression, input.line, input.char_expression);
				break;

			// Forwards to limiting method
			case CAS::Command_t::LIMIT:
				warn_on_args();
				impl->eval_limit(input.expression, input.line, input.char_expression);
				break;

			// Parses function for differentiation function
			case CAS::Command_t::DIFFERENTIATE: {
				error_with_no_args();

				size_t i = 0;
				auto&& a = input.args;

				_detail::Differentiation_t diff{1, true};

				// Search for "partial"
				if (a.size() >= 7) {
					for (char c : "partial") {
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

				// Set the n value based on text
				if (_util::is_number(a[i])) {
					// strtoull ignores characters after the number
					diff.n = std::strtoull(a.c_str() + i, nullptr, 10);
				}
				else {
					throw Create_Temp_Error("Expected number");
				}

				impl->eval_differentiate(input.expression, diff, input.line, input.char_expression);
				break;
			}

			case CAS::Command_t::INTEGRATE:
				warn_on_args();
				impl->eval_integrate(input.expression, input.line, input.char_expression);
				break;

			case CAS::Command_t::INTEGRATE_DEFINITE: {
				error_with_no_args();

				auto&& a = input.args;
				size_t i = 0;

				_detail::Definite_Integration_t def_int;

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

				impl->eval_integrate_definite(input.expression, def_int, input.line,
				                              input.char_expression);
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
