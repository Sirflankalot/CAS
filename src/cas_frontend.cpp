#include "cas_frontend.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

CAS::User_Input_t CAS::separate_input(const std::string& input) {
	static const std::unordered_map<std::string, CAS::Command_t> command_map{
	    //
	    {"quit", CAS::Command_t::QUIT},
	    {"simplify", CAS::Command_t::SIMPLIFY},
	    {"solve", CAS::Command_t::SOLVE},
	    {"substitute", CAS::Command_t::SUBSTITUTE},
	    {"interpolate", CAS::Command_t::INTERPOLATE},
	    {"root", CAS::Command_t::ROOT},
	    {"limit", CAS::Command_t::LIMIT},
	    {"differentiate", CAS::Command_t::DIFFERENTIATE},
	    {"integrate", CAS::Command_t::INTEGRATE},
	    {"integrate_definite", CAS::Command_t::INTEGRATE_DEFINITE}
	    //
	};

	CAS::Command_t command{CAS::Command_t::NONE};
	std::string args;
	std::string expression;

	bool has_command{false};
	bool has_args{true};

	size_t i = 0; // current character index in the input string

	// Checks if the string has the character needed for a command
	auto colon_it = std::find(input.begin(), input.end(), ':');
	has_command   = colon_it != input.end();

	// If there is a colon, parse the command
	if (has_command) {
		// Reads the first word terminated by a space or : as the command
		// Finding a colon means there are no arguments
		std::string cmd_str;
		for (; i < input.size(); ++i) {
			if (input[i] == ' ') {
				i += 1;
				break;
			}
			else if (input[i] == ':') {
				has_args = false;
				break;
			}
			cmd_str.push_back(input[i]);
		}

		// If command is found in map, set the command
		auto cmd_it = command_map.find(cmd_str);
		if (cmd_it == command_map.end()) {
			// TODO: This is dumb
			throw std::runtime_error("Command not found");
		}
		command = (*cmd_it).second;

		// Add arguments if applicable
		if (has_args) {
			for (; i < input.size(); ++i) {
				if (input[i] == ':') {
					i += 1;
					break;
				}
				args.push_back(input[i]);
			}
		}
	}

	// skips the first space and adds the remaining characters in input to the string expression
	// which holds the mathematical expression
	// to be solved
	expression = input.substr(i);

	return CAS::User_Input_t{std::move(command), std::move(args), std::move(expression)};
}
