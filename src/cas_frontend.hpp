#pragma once

#include <string>

namespace CAS {
	/*Provides an enum with all legal commands in the C++ CAS language and a function to convert the
	std::string command into its
	commands equivalent. This allows for switch statements to easily be used in the function
	handle_inputs.*/
	enum class Command_t {
		NONE = 0,
		QUIT,
		SIMPLIFY,
		SOLVE,
		SUBSTITUTE,
		INTERPOLATE,
		ROOT,
		LIMIT,
		DIFFERENTIATE,
		INTEGRATE,
		INTEGRATE_DEFINITE
	};

	// A simple struct to return user input from
	struct User_Input_t {
		CAS::Command_t command;
		std::string args;
		std::string expression;
	};

	// get_input: std::istream& -> std::string
	// takes an std::istream and gets a line from it which is then returned with no trailing space
	// or newline
	// std::cin is not explicitly used so that this function can also read from std::fstream and
	// std::stringstream objects
	std::string get_input(std::istream&);

	// separate_input: cosnt std::string& -> void
	// takes in a string and two string references
	// separates the first string into a command and math expression
	User_Input_t separate_input(const std::string&);
}
