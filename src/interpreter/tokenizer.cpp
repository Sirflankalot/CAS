#include "cas-impl.hpp"
#include "util/util.hpp"

#include <array>
#include <iostream>
#include <utility>

CAS::_detail::Token_List_t CAS::_detail::tokenize(const std::string& input_str) {
	CAS::_detail::Token_List_t token_vals;

	auto&& token_list = token_vals.tokens;
	auto&& token_data = token_vals.data;

	size_t i = 0;

	auto parse_number = [& oi = i, &input_str, &token_data, &token_list ]()->bool {
		size_t i = oi + 1;
		std::string number;
		if (i < input_str.size() && !(_util::is_number(input_str[i]) || input_str[i] == '.')) {
			return false;
		}

		number += input_str[i - 1];

		for (; i < input_str.size() && _util::is_number(input_str[i]); ++i) {
			number += input_str[i];
		}

		if (i < input_str.size() && input_str[i] == '.') {
			i += 1;
			number += '.';
			for (; i < input_str.size() && _util::is_number(input_str[i]); ++i) {
				number += input_str[i];
			}
			if (i < input_str.size() && input_str[i] == '.') {
				// Multiple dots
			}
		}

		token_data.emplace_back(std::move(number));
		token_list.emplace_back(CAS::_detail::Token_t::LITERAL, token_data.size() - 1);
		oi = i - 1;

		return true;
	};

	using Token = CAS::_detail::Token_t;

	for (; i < input_str.size(); ++i) {
		switch (input_str[i]) {
			// Parse various symbols
			case '(':
				token_list.emplace_back(Token::LEFT_PAREN);
				break;
			case ')':
				token_list.emplace_back(Token::RIGHT_PAREN);
				break;
			case '[':
				token_list.emplace_back(Token::LEFT_BRACKET);
				break;
			case ']':
				token_list.emplace_back(Token::RIGHT_BRACKET);
				break;
			case '{':
				token_list.emplace_back(Token::LEFT_BRACE);
				break;
			case '}':
				token_list.emplace_back(Token::RIGHT_BRACE);
				break;
			case '<':
				token_list.emplace_back(Token::LEFT_CARROT);
				break;
			case '>':
				token_list.emplace_back(Token::RIGHT_CARROT);
				break;
			case '=':
				token_list.emplace_back(Token::EQUALS);
				break;
			case '*':
				token_list.emplace_back(Token::STAR);
				break;
			case '/':
				token_list.emplace_back(Token::SLASH);
				break;
			case '^':
				token_list.emplace_back(Token::CARROT);
				break;
			case '|':
				token_list.emplace_back(Token::PIPE);
				break;
			case '%':
				token_list.emplace_back(Token::PERCENTAGE);
				break;
			case '~':
				token_list.emplace_back(Token::TILDE);
				break;
			case '!':
				token_list.emplace_back(Token::BANG);
				break;
			case ';':
				token_list.emplace_back(Token::SEMICOLON);
				break;
			// Parse symbols that may be numbers
			case '+': {
				token_list.emplace_back(Token::PLUS);
				break;
			}
			case '-': {
				bool is_number = parse_number();
				if (!is_number) {
					token_list.emplace_back(Token::MINUS);
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
			case '.': {
				parse_number();
				break;
			}

			case ' ':
				break;
			// Parse a name
			default: {
				if (_util::is_letter(input_str[i])) {
					std::string name;
					for (; i < input_str.size() && _util::is_letter(input_str[i]); ++i) {
						name += input_str[i];
					}
					i -= 1;
					token_data.emplace_back(std::move(name));
					token_list.emplace_back(Token::IDENTIFIER, token_data.size() - 1);
				}
				else {
					token_list.emplace_back(Token::NONE);
				}
				break;
			}
		}
	}

	return token_vals;
}

void CAS::_detail::print_tokens(const CAS::_detail::Token_List_t& token_list) {
	constexpr static std::array<std::pair<const char*, char>, 22> token_strings = {
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
	    std::make_pair("    SEMICOLON", ';'), //
	};

	for (auto&& token : token_list.tokens) {
		const size_t index = static_cast<size_t>(token.type);
		const char* name   = token_strings[index].first;
		const char val     = token_strings[index].second;
		if (token.type == CAS::_detail::Token_t::IDENTIFIER ||
		    token.type == CAS::_detail::Token_t::LITERAL) {
			std::cerr << name << " | " << token_list.data[token.data_loc] << '\n';
		}
		else {
			std::cerr << name << " | " << val << '\n';
		}
	}
}
