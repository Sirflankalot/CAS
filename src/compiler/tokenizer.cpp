#include "cas-impl.hpp"
#include "util/util.hpp"

#include <algorithm>
#include <array>
#include <cinttypes>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <utility>

CAS::_detail::Token_List_t CAS::_detail::tokenize(const std::string& input_str, size_t line_num,
                                                  size_t char_num) {
	// Return value
	CAS::_detail::Token_List_t token_vals;

	// Forwarding references to each component (for ease of access)
	auto&& token_list = token_vals.tokens;
	auto&& token_data = token_vals.data;

	// Index into input_str
	size_t i = 0;

	// Lambda function that allows the line numbers and char numbers be added to the token without
	// unnessisary repitition of code. The offset allows for compensation for the length of a token.
	// For example, if a token is "hello" when this function is called i will point to the last
	// character in the token. Adding an offset of -4 will make it point to the first character in
	// the token. In addition, it also changes from a 0 indexed to a 1 indexed character number.
	auto add_token = [&](CAS::_detail::Token_t::Type_List type,
	                     size_t data_ptr = static_cast<size_t>(-1), int64_t char_offset = 0) {
		token_list.emplace_back(type, data_ptr, line_num,
		                        char_num + i + 1UL + static_cast<size_t>(char_offset));
	};

	// Lambda function that tries to parse a number. If it is not a number, returns false with no
	// change to the index. If it is a number, adds the appropriate tokens and leaves i pointing to
	// the last character in the number.
	auto parse_number = [& oi = i, &input_str, &token_data, &token_list, &add_token ]()->bool {
		// Internal copy of i (oi is a reference to the Original I)
		size_t i = oi;

		// Resulting number
		std::string number;

		// If the number starts with a minus or dot, process that dot
		if (_util::char_is_in(input_str[i], '-', '.')) {
			i += 1;
			// If the character after the minus isn't a number or a dot or if the character after a
			// dot isn't a number, we have not found a number
			if (i < input_str.size() &&
			    !(_util::is_number(input_str[i]) ||
			      (input_str[i - 1] == '-' && input_str[i] == '.'))) {
				return false;
			}

			// Add starting character
			number += input_str[i - 1];
		}

		// Iterate and add to string until i doesn't point to a number
		for (; i < input_str.size() && _util::is_number(input_str[i]); ++i) {
			number += input_str[i];
		}

		// If i points to a '.', then add the dot, and then add the rest of the number
		if (i < input_str.size() && input_str[i] == '.') {
			i += 1;
			number += '.';
			for (; i < input_str.size() && _util::is_number(input_str[i]); ++i) {
				number += input_str[i];
			}
			// TODO: Throw error if there is multiple dots
			if (i < input_str.size() && input_str[i] == '.') {
				// Multiple dots
			}
		}

		// Add token data and the token itself
		token_data.emplace_back(std::move(number));
		add_token(CAS::_detail::Token_t::LITERAL, token_data.size() - 1);

		// Return the original i to point to the last character in the number
		oi = i - 1;

		return true;
	};

	// Alias to minimise typing
	using Token = CAS::_detail::Token_t;

	for (; i < input_str.size(); ++i) {
		switch (input_str[i]) {
			// Parse various symbols
			case '(':
				add_token(Token::LEFT_PAREN);
				break;
			case ')':
				add_token(Token::RIGHT_PAREN);
				break;
			case '[':
				add_token(Token::LEFT_BRACKET);
				break;
			case ']':
				add_token(Token::RIGHT_BRACKET);
				break;
			case '{':
				add_token(Token::LEFT_BRACE);
				break;
			case '}':
				add_token(Token::RIGHT_BRACE);
				break;
			case '<':
				add_token(Token::LEFT_CARROT);
				break;
			case '>':
				add_token(Token::RIGHT_CARROT);
				break;
			case '=':
				add_token(Token::EQUALS);
				break;
			case '*':
				add_token(Token::STAR);
				break;
			case '/':
				add_token(Token::SLASH);
				break;
			case '^':
				add_token(Token::CARROT);
				break;
			case '|':
				add_token(Token::PIPE);
				break;
			case '%':
				add_token(Token::PERCENTAGE);
				break;
			case '~':
				add_token(Token::TILDE);
				break;
			case '!':
				add_token(Token::BANG);
				break;
			case ',':
				add_token(Token::COMMA);
				break;
			case ';':
				add_token(Token::SEMICOLON);
				break;
			case '+':
				add_token(Token::PLUS);
				break;

			// Parse '-' which may be the start to a number
			case '-': {
				bool is_number = parse_number();
				if (!is_number) {
					add_token(Token::MINUS);
				}
				break;
			}

			// Parse a number
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
				parse_number();
				break;

			// Pass over spaces
			case ' ':
				break;

			// Parse a name or unknown symbol
			default: {
				if (_util::is_letter(input_str[i])) {
					// String with the name of the variable
					std::string name;

					// Loop until a name isn't found
					for (; i < input_str.size() && _util::is_letter(input_str[i]); ++i) {
						name += input_str[i];
					}

					// i should point to the last character
					i -= 1;

					// Cache length because it is used after a move
					int64_t len = static_cast<int64_t>(name.size());

					// Add name to token data
					token_data.emplace_back(std::move(name));

					// Add a token with the correct offset
					add_token(Token::IDENTIFIER, token_data.size() - 1, -(len - 1));
				}
				else {
					add_token(Token::NONE);
				}
				break;
			}
		}
	}

	// Return the tokens
	return token_vals;
}

void CAS::_detail::print_tokens(const CAS::_detail::Token_List_t& token_list) {
	// What's the point of doing anything if we don't have any tokens to print
	if (token_list.tokens.size() == 0) {
		return;
	}

	// Array with mappings from the token enum to data about the name
	constexpr static std::array<std::pair<const char*, char>, 23> token_strings = {{
	    std::make_pair("         NONE", ' '), //
	    std::make_pair("   IDENTIFIER", 'f'), //
	    std::make_pair("      LITERAL", '2'), //
	    std::make_pair("   LEFT_PAREN", '('), //
	    std::make_pair("  RIGHT_PAREN", ')'), //
	    std::make_pair(" LEFT_BRACKET", '['), //
	    std::make_pair("RIGHT_BRACKET", ']'), //
	    std::make_pair("   LEFT_BRACE", '{'), //
	    std::make_pair("  RIGHT_BRACE", '}'), //
	    std::make_pair("  LEFT_CARROT", '<'), //
	    std::make_pair(" RIGHT_CARROT", '>'), //
	    std::make_pair("       EQUALS", '='), //
	    std::make_pair("         PLUS", '+'), //
	    std::make_pair("        MINUS", '-'), //
	    std::make_pair("         STAR", '*'), //
	    std::make_pair("        SLASH", '/'), //
	    std::make_pair("       CARROT", '^'), //
	    std::make_pair("         PIPE", '|'), //
	    std::make_pair("   PERCENTAGE", '%'), //
	    std::make_pair("        TILDE", '~'), //
	    std::make_pair("         BANG", '!'), //
	    std::make_pair("        COMMA", ','), //
	    std::make_pair("    SEMICOLON", ';'), //
	}};

	// Find the amount of characters needed to represent the largest line number
	int max_line_size = static_cast<int>(std::ceil(std::log10(token_list.tokens[0].line_num)));

	// Find the amount of characters needed to represent the largest character number by finding the
	// element with the largest char number, then figuring out how many characters are needed to
	// represent that.
	int max_char_size = static_cast<int>(std::ceil(std::log10(
	    std::max_element(token_list.tokens.begin(), token_list.tokens.end(), [](auto a, auto b) {
		    return (a.char_num < b.char_num);
		})->char_num)));

	// Loop through all tokens
	for (const auto& token : token_list.tokens) {
		const size_t index = static_cast<size_t>(token.type);
		const char* name   = token_strings[index].first;
		const char val     = token_strings[index].second;

		// Print line and char numbers at correct width
		std::cerr << std::setfill('0') << std::setw(max_line_size) << token.line_num << ':'
		          << std::setw(max_char_size) << token.char_num << ": \t" << name << " \"";

		// If it is a literal or an identifier, print the data value
		if (token.type == CAS::_detail::Token_t::IDENTIFIER ||
		    token.type == CAS::_detail::Token_t::LITERAL) {
			std::cerr << token_list.data[token.data_loc] << "\"\n";
		}
		// Otherwise print the appropriate character
		else {
			std::cerr << val << "\"\n";
		}
	}
}
