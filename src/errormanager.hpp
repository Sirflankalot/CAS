#pragma once

#include <cinttypes>
#include <cstdlib>
#include <string>
#include <vector>

namespace CAS {
    // An enum to hold error types
    enum class Error_t {
        // error types
    };

    struct Error {
        Error(std::string output, Error_t type, uint64_t n_line, uint16_t n_char);
        std::string output;
        CAS::Error_t type;
        uint64_t n_line{0};
        uint16_t n_char{0}; // character number in line where error starts
    };

    class Error_Manager {
      public:
        // default constructor
        Error_Manager();

        // constructor
        Error_Manager(const CAS::Error_Manager&) = default;
        Error_Manager(CAS::Error_Manager&&)      = default;
        Error_Manager& operator=(const CAS::Error_Manager&) = default;
        Error_Manager& operator=(CAS::Error_Manager&&) = default;

        // adds an error to the error list
        void add_error(CAS::Error);

        size_t count_errors(const CAS::Error_t);

      private:
        // Vector holding the Errors for the current user input
        std::vector<CAS::Error> error_list;
    };
}
