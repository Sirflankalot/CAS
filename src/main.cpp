#include <iostream>
#include <string>

#include "cas_frontend.hpp"


int main(int argc, char * argv[])
{
	std::string command(""), expression("");
	bool quit = false;

	std::cout << "C++ CAS\nversion 0.0\n";
	while(true)//primary I/O loop
	{
		std::cout << "> ";
		CAS::separate_input(CAS::get_input(std::cin), command, expression);
		CAS::handle_inputs(CAS::make_command(command), expression);

		//if the handle_inputs function runs into the quit command it will set bool quit to true and execution will end
		if (quit) 
		{
		    break;
		}
	}

	return 0;
}
