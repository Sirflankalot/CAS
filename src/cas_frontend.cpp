#include "cas_frontend.hpp"

#include <cstdlib>
#include <iostream>

std::string CAS::get_input(std::istream& input_stream)
{
	std::string input_line;
	std::getline(input_stream, input_line, '\n');
	return input_line;
}

CAS::User_Input_t CAS::separate_input(const std::string& input)
{
	CAS::User_Input_t ret;

	size_t i = 0; // current character index in the input string

	// sets the value of the command string variable to the first word in the input string
	for (i = 0; i < input.size(); ++i)
	{
		if (input[i] != ' ')
		{
			ret.command.push_back(input[i]);
		}

		// stops adding letters to command after reaching the first space in input
		else
		{
			i = i + 1;
			break;
		}
	}

	// Resereve the space needed for the expression
	ret.expression.reserve(input.size() - i);

	// skips the first space and adds the remaining characters in input to the string expression
	// which holds the mathematical expression
	// to be solved
	for (; i < input.size(); i++)
	{
		ret.expression.push_back(input[i]);
	}

	return ret;
}

CAS::command CAS::make_command(const std::string& raw_command)
{
	// use if/else if to convert the std::string raw_command to its CAS::commands counterpart for
	// use with CAS::handle_inputs
	/* prototype:

	if(raw_command == "example") return CAS::commands::example;
	else if(raw_command == "other") return CAS::commands::other;
	else return CAS::commands::invalid_command;
	*/
}

void CAS::handle_inputs(const CAS::command& command, const std::string& argument)
{
	(void) argument;

	switch (command)
	{
		default:
			break;
	}
}
