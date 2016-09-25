#include "cas-errormanager.hpp"

CAS::Error::Error(const std::string output, Error_t type, const unsigned long n_line, const unsigned short n_char)
    : output(output), type(type), n_line(n_line), n_char(n_char)
{
}
