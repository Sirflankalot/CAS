#include <iostream>
#include <string>

#include "cas_frontend.hpp"

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	bool quit = false;

	std::cout << "C++ CAS\nversion 0.0\n";

	// Non-owning pointer to the input type
	std::istream* input_stream = &std::cin;

	while (true) // primary I/O loop
	{
		std::cout << "> ";

		// Read string from user
		std::string raw_input;
		std::getline(*input_stream, input_line, '\n');

		// Parse the command, send it to handler
		auto broken_down = CAS::separate_input(raw_input);
		CAS::handle_inputs(broken_down.command, broken_down.expression);

		// TODO: Impliment error management system
		if (quit)
		{
			break;
		}
	}

	return 0;
}
