#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "cas_frontend.hpp"
#include "cpp-cas.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {
	// File constructs
	bool using_file{false};
	std::string filename{};

	// Two argments mean a file has been supplied
	if (argc == 2) {
		using_file = true;
		filename   = argv[1];
	}
	else if (argc != 1) {
		std::cerr << "Not enough arguments.\n";
	}

	// Non-owning pointer to the input stream
	std::istream* in_stream;

	// File stream
	std::ifstream file_stream;

	// Set streams accordingly if using file
	if (using_file) {
		file_stream.open(filename);
		if (!file_stream) {
			std::cerr << "File " << filename << " cannot be opened\n";
			return 1;
		}

		in_stream = &file_stream;
	}
	else {
		in_stream = &std::cin;
	}

	// Non-owning pointer to output stream
	std::ostream* out_stream{&std::cout};

	*out_stream << "C++ CAS\n"
	               "version 0.0\n";

	// Actual interpreter
	CAS::Interpreter cas;

	// primary I/O loop
	bool quit{false};
	while (!quit) {
		if (!using_file) {
			*out_stream << "> ";
		}

		// Read string from user
		std::string raw_input;
		std::getline(*in_stream, raw_input, '\n');

		try {
			// Parse the command, send it to handler
			auto broken_down = CAS::separate_input(raw_input);

			if (broken_down.command == CAS::Command_t::QUIT) {
				quit = true;
				continue;
			}

			cas.input(broken_down);
		}
		catch (CAS::_util::Temp_Error& e) {
			std::cerr << e.what() << '\n';
		}

		// TODO: Impliment error management system
	}

	return 0;
}
