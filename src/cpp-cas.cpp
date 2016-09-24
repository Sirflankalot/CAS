#include "cpp-cas.hpp"
#include "cas-impl.hpp"
#include "util.hpp"

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
				while (i < a.size() && _util::is_whitespace(a[i])) {
					i += 1;
				}

				// check for "for "
				if (!(i + 4 < a.size())) {
					throw std::runtime_error("Expected \"for\"");
				}
				for (char c : "for ") {
					// If search is not in string, and search isn't null terminator
					if (a[i] != c && c != '\0') {
						throw std::runtime_error("Expected \"for\"");
					}
					i += 1;
				}

				bool finished{false};

				// Collect variables
				while (!finished) {
					std::string var;

					while (i < a.size() && !_util::is_whitespace(a[i])) {
						var.push_back(a[i]);
						i += 1;
					}

					if (i < a.size()) {
						finished = true;
					}
					else {
						i += 1;
						// Check for 'and '
						if (!(i + 4 < a.size())) {
							throw std::runtime_error("Expected \"and\"");
						}
						for (char c : "and ") {
							if (a[i] != c && c != '\0') {
								throw std::runtime_error("Expected \"and\"");
							}
							i += 1;
						}
					}

					variables.emplace_back(std::move(var));
				}

				impl->eval_solve(input.expression.c_str(), variables);
				break;
			}

			// TODO: Impliment parsing for interpolate, defferentiate, and integrate_definite

			case CAS::Command_t::SUBSTITUTE:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::INTERPOLATE:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::ROOT:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::LIMIT:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::DIFFERENTIATE:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::INTEGRATE:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			case CAS::Command_t::INTEGRATE_DEFINITE:
				warn_on_args();
				impl->eval_calculate(input.expression.c_str());
				break;

			// Get the compiler to shut up
			case CAS::Command_t::QUIT:
				break;
		}
	}
}
