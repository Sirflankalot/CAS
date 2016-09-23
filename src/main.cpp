#include <iostream>
#include <string>

#include "cas_frontend.hpp"

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	bool quit = false;

	std::cout << "C++ CAS\nversion 0.0\n";

	while (true) // primary I/O loop
	{
		std::cout << "> ";

		std::string raw_input = CAS::get_input(std::cin);
		auto broken_down      = CAS::separate_input(raw_input);
		CAS::handle_inputs(broken_down.command, broken_down.expression);

		// if the handle_inputs function runs into the quit command it will set bool quit to true
		// and execution will end
		if (quit)
		{
			break;
		}
	}

	return 0;
}
