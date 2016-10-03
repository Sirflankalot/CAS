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
	CAS::_detail::Token_List_t token_vals;

	auto&& token_list = token_vals.tokens;
	auto&& token_data = token_vals.data;

	size_t i = 0;

	auto add_token = [&](CAS::_detail::Token_t::Type_List type,
	                     size_t data_ptr = static_cast<size_t>(-1), int64_t char_offset = 0) {
		token_list.emplace_back(type, data_ptr, line_num, char_num + i + 1UL + char_offset);
	};

	auto parse_number = [& oi = i, &input_str, &token_data, &token_list, &add_token ]()->bool {
		size_t i = oi;
		std::string number;
		if (_util::char_is_in(input_str[i], '-', '.')) {
			i += 1;
			if (i < input_str.size() && !(_util::is_number(input_str[i]) || input_str[i] == '.')) {
				return false;
			}

			number += input_str[i - 1];
		}

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
		add_token(CAS::_detail::Token_t::LITERAL, token_data.size() - 1);
		oi = i - 1;

		return true;
	};

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
			case ';':
				add_token(Token::SEMICOLON);
				break;
			// Parse symbols that may be numbers
			case '+':
				add_token(Token::PLUS);
				break;
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
					size_t len = name.size();
					token_data.emplace_back(std::move(name));
					add_token(Token::IDENTIFIER, token_data.size() - 1, -(len - 1));
				}
				else {
					add_token(Token::NONE);
				}
				break;
			}
		}
	}

	return token_vals;
}

void CAS::_detail::print_tokens(const CAS::_detail::Token_List_t& token_list) {
	if (token_list.tokens.size() == 0) {
		return;
	}

	constexpr static std::array<std::pair<const char*, char>, 22> token_strings = {{
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
	}};

	int max_line_size = static_cast<int>(std::ceil(std::log10(token_list.tokens[0].line_num)));
	int max_char_size = static_cast<int>(std::ceil(std::log10(
	    std::max_element(token_list.tokens.begin(), token_list.tokens.end(), [](auto a, auto b) {
		    return (a.char_num < b.char_num);
		})->char_num)));

	for (auto&& token : token_list.tokens) {
		const size_t index = static_cast<size_t>(token.type);
		const char* name   = token_strings[index].first;
		const char val     = token_strings[index].second;

		std::cerr << std::setfill('0') << std::setw(max_line_size) << token.line_num << ':'
		          << std::setw(max_char_size) << token.char_num << ":\t";

		if (token.type == CAS::_detail::Token_t::IDENTIFIER ||
		    token.type == CAS::_detail::Token_t::LITERAL) {
			std::cerr << name << " \"" << token_list.data[token.data_loc] << "\"\n";
		}
		else {
			std::cerr << name << " \"" << val << "\"\n";
		}
	}
}
