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

		// Dummy structs to represent future concepts
		struct Variable_t {};
		struct Token_t {};
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
		Token_t                 tokenize (const char*);
		AST_Head_Node_t            parse (const std::vector<Token_t>&);
		std::vector<IR_Token_t> code_gen (const AST_Head_Node_t&);
		void                    optimize (std::vector<IR_Token_t>&);
		std::string             reserect (const std::vector<IR_Token_t>&);
		// clang-format on

		// Implimentation API
		class interpreter_impl {
		  private:
			std::vector<std::unique_ptr<Variable_t>> variables;

		  public:
			// Evaluate functions that a handler can call
			void eval_calculate(const char*);
			void eval_simplify(const char*);
			void eval_solve(const char*, std::vector<std::string>&);
			void eval_substitute(const char*);
			void eval_interpolate(const char*, Interpolation_t);
			void eval_root(const char*);
			void eval_limit(const char*);
			void eval_differentiate(const char*, Differentiation_t&);
			void eval_integrate(const char*);
			void eval_integrate_definite(const char*, Definite_Integration_t&);

			void clear_all_state();
		};
	}
}
