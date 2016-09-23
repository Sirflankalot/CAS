#include <string>

#ifndef CAS_FRONTEND
#define CAS_FRONTEND

namespace CAS
{
	// get_input: std::istream -> std::string
	// takes an std::iostream and gets a line from it which is then returned with no trailing space
	// or newline
	// std::cin is not explicitly used so that this function can also read from std::fstream and
	// std::stringstream objects
	std::string get_input(std::istream&);

	// separate_input: std::string &std::string &std::string -> void
	// takes in a string and two string references
	// separates the first string into a command and math expression
	void separate_input(const std::string&, std::string&, std::string&);

	/*Provides an enum with all legal commands in the C++ CAS language and a function to convert the
	std::string command into its
	commands equivalent. This allows for switch statements to easily be used in the function
	handle_inputs.*/
	enum command
	{
		// legal commands in the C++ CAS language will be listed here and are used by the
		// handle_inputs function
	};
	CAS::command make_command(const std::string&);

	// handle_input: const std::string const std::string -> void
	// take the appropriate action based on the command passed in and using the given math
	// expression
	void handle_inputs(const CAS::command&, const std::string&);
}

#endif // CAS_FRONTEND
