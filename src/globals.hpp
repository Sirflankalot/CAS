#pragma once

#include <iosfwd>

// Global Variables //
// extern ErrHandler_t
extern std::istream* in_stream;    // Non-owning pointer to input stream
extern std::ostream* out_stream;   // Non-owning pointer to output stream
extern std::ostream* error_stream; // Non-owning pointer to error stream

// Useful Macros and Version Numbers
#define CAS_MAJOR_VERSION 0
#define CAS_MINOR_VERSION 0
#define CAS_VERSION 0.0
#define CAS_VERSION_STR "0.0"
#define CAS_COPYRIGHT "Copyright (c) 2016 Connor Fitzgerald and Mohit D. Patel"

#define CAS_HELP_TEXT " [filename]\n"

// clang-format off
#define CAS_COMMAND_HELP_TEXT                                                                        \
	"Command            | Arguments        | Use\n"                                                  \
	"------------------ | ---------------- | -----------\n"                                          \
	"quit               | none             | Quits.\n"                                               \
	"help               | none             | Displays the help menu\n"                               \
	"calculate          | none             | Default behavior, evaluates\n"                          \
	"                   |                  | the expression normally\n"                              \
	"simplify           | none             | Simplifies equation without moving \n"                  \
	"                   |                  | variables from their side\n"                            \
	"solve              | for x [and y]    | Tries to isolate the variables provided.\n"             \
	"                   |                  | Returns a functor\n"                                    \
	"substitute         | none             | Allows functions to remain unevaluated. ex. `f(7)`\n"   \
	"                   |                  | will be `(7) + 2` instead of `9`.\n"                    \
	"interpolate        | constant or      | \n"                                                     \
	"                   | linear or cosine | \n"                                                     \
	"                   | or spline        | Allows tables to be evaluated with interpolation.\n"    \
	"root               | none             | Takes the root of a quadratic or a cubic functor\n"     \
	"limit              | none             | Finds the domain limits of a single variable functor\n" \
	"differentiate      | [partial]        | Calculates the n-th [partial] derivative of \n"         \
	"                   | 1st | 2nd...     | the provided expression\n"                              \
	"integrate          | none             | Calculates the indefinite integral\n"                   \
	"integrate_definite | x {a,b}          | Returns the definite integral of the given function\n"  \
	"                   |                  | for x in the range a to b\n"
//clang-format on