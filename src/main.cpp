#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "cas_frontend.hpp"
#include "cpp-cas.hpp"
#include "globals.hpp"
#include "util.hpp"

std::istream* in_stream{&std::cin};
std::ostream* out_stream{&std::cout};
std::ostream* error_stream{&std::cerr};

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
		*error_stream << "Too many arguments.\n"
		              << "Usage: " << argv[0] << CAS_HELP_TEXT;
		return 1;
	}

	// File stream
	std::ifstream file_stream;

	// Set streams accordingly if using file
	if (using_file) {
		file_stream.open(filename);
		if (!file_stream) {
			*error_stream << "File " << filename << " cannot be opened\n";
			return 1;
		}

		in_stream = &file_stream;
	}

	*out_stream << "C++ CAS\n"
	            << CAS_COPYRIGHT "\n"
	            << "version " CAS_VERSION_STR "\n";

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
			*error_stream << e.what() << '\n';
		}

		// TODO: Impliment error management system
	}

	return 0;
}
