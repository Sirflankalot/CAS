#include "errormanager.hpp"

CAS::Error::Error(std::string output_in, Error_t type_in, uint64_t n_line_in, uint16_t n_char_in)
    : output(std::move(output_in)), type(type_in), n_line(n_line_in), n_char(n_char_in) {}
