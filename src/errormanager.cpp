#include "errormanager.hpp"

CAS::Error::Error(std::string output_in, Error_t type_in, uint64_t n_line_in, uint16_t n_char_in)
    : output(std::move(output_in)), type(type_in), n_line(n_line_in), n_char(n_char_in) {}

CAS::Error_Manager::Error_Manager() {}

void CAS::Error_Manager::add_error(CAS::Error error) {
	error_list.push_back(error);
	total_errors++;
}

size_t CAS::Error_Manager::get_total_errors() {
	return total_errors;
}

size_t CAS::Error_Manager::count_errors(const CAS::Error_t error_type) {
	size_t n_errors{0};
	for (CAS::Error e : error_list) {
		if (e.type == error_type) {
			n_errors++;
		}
	}
	return n_errors;
}
