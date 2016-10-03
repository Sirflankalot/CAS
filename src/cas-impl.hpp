#pragma once

#include "cas_frontend.hpp"
#include "cpp-cas.hpp"

#include <memory>
#include <vector>

namespace CAS {
	namespace _detail {
		// Enum with interpolation types
		enum class Interpolation_t { NONE = 0, CONSTANT, LINEAR, COSINE, SPLINE };
		// Struct with differentiation information
		struct Differentiation_t {
			size_t n     = 1;
			bool partial = false;
		};
		// Struct with definite integration information
		struct Definite_Integration_t {
			std::string variable;
			std::string start_num;
			std::string end_num;
		};

		struct Token_t {
			size_t data_loc;
			size_t line_num;
			size_t char_num;
			enum Type_List {
				NONE       = 0,     // Nothing
				IDENTIFIER = 1,     // f in f(2)
				LITERAL    = 2,     // 2 in f(2)
				                    //
				LEFT_PAREN    = 3,  // (
				RIGHT_PAREN   = 4,  // )
				LEFT_BRACKET  = 5,  // [
				RIGHT_BRACKET = 6,  // ]
				LEFT_BRACE    = 7,  // {
				RIGHT_BRACE   = 8,  // }
				LEFT_CARROT   = 9,  // <
				RIGHT_CARROT  = 10, // >
				EQUALS        = 11, // =
				PLUS          = 12, // +
				MINUS         = 13, // -
				STAR          = 14, // *
				SLASH         = 15, // /
				CARROT        = 16, // ^
				PIPE          = 17, // |
				PERCENTAGE    = 18, // %
				TILDE         = 19, // ~
				BANG          = 20, // !
				                    //
				SEMICOLON = 21      // ;
			} type;

			Token_t(Type_List it, size_t iloc, size_t ln, size_t cn)
			    : data_loc(iloc), line_num(ln), char_num(cn), type(it) {}
		};

		struct Token_List_t {
			std::vector<Token_t> tokens;
			std::vector<std::string> data;
		};

		// Dummy structs to represent future concepts
		struct Variable_t {};
		struct AST_Head_Node_t {};
		struct IR_Token_t {};

		// Stand alone functions that the compiler will use
		// Basic flow is as follows:
		//
		// Input         |  Function   | Output
		// Text         -> Tokenizer  -> Token List
		// Token List   -> Parser     -> Abstract Syntax Tree (AST)
		// AST          -> Code Gen.  -> Intermediate Represenation (IR)
		// IR           -> Optimizer  -> IR (simpler)
		// IR (simpler) -> Reserecter -> Text
		//
		// clang-format off
		Token_List_t            tokenize (const std::string&, size_t line_num, size_t char_num);
		AST_Head_Node_t         parse    (const Token_List_t&);
		std::vector<IR_Token_t> code_gen (const AST_Head_Node_t&);
		void                    optimize (std::vector<IR_Token_t>&);
		std::string             reserect (const std::vector<IR_Token_t>&);
		// clang-format on
		void print_tokens(const Token_List_t&);

		// Implimentation API
		class interpreter_impl {
		  private:
			std::vector<std::unique_ptr<Variable_t>> variables;

		  public:
			// Evaluate functions that a handler can call
			void eval_calculate(const std::string&, size_t line_num, size_t char_num);
			void eval_simplify(const std::string&, size_t line_num, size_t char_num);
			void eval_solve(const std::string&, std::vector<std::string>&, size_t line_num,
			                size_t char_num);
			void eval_substitute(const std::string&, size_t line_num, size_t char_num);
			void eval_interpolate(const std::string&, Interpolation_t, size_t line_num,
			                      size_t char_num);
			void eval_root(const std::string&, size_t line_num, size_t char_num);
			void eval_limit(const std::string&, size_t line_num, size_t char_num);
			void eval_differentiate(const std::string&, Differentiation_t&, size_t line_num,
			                        size_t char_num);
			void eval_integrate(const std::string&, size_t line_num, size_t char_num);
			void eval_integrate_definite(const std::string&, Definite_Integration_t&,
			                             size_t line_num, size_t char_num);

			void clear_all_state();
		};
	}
}
