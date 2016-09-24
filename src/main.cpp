#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "cas_frontend.hpp"
#include "cpp-cas.hpp"

int main(int argc, char* argv[]) {
	// File constructs
	bool using_file{false};
	std::string filename{""};

	// Two argments mean a file has been supplied
	if (argc == 2) {
		using_file = true;
		filename   = argv[1];
	}
	else if (argc != 1) {
		std::cerr << "Not enough arguments";
	}

	// Non-owning pointer to the input type
	std::istream* input_stream_ptr;

	// File stream
	std::ifstream file_stream;

	// Set streams accordingly if using file
	if (using_file) {
		file_stream.open(filename);
		if (!file_stream) {
			std::cerr << "File " << filename << " cannot be opened";
			return 1;
		}

		input_stream_ptr = &file_stream;
	}
	else {
		input_stream_ptr = &std::cin;
	}

	std::cout << "C++ CAS\nversion 0.0\n";

	bool quit{false};
	while (!quit) // primary I/O loop
	{
		if (!using_file) {
			std::cout << "> ";
		}

		// Read string from user
		std::string raw_input;
		std::getline(*input_stream_ptr, raw_input, '\n');

		// Parse the command, send it to handler
		auto broken_down = CAS::separate_input(raw_input);
		CAS::handle_inputs(broken_down.command, broken_down.expression);

		// TODO: Impliment error management system
	}

	return 0;
}
