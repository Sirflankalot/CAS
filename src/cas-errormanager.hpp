#pragma once

#include <string>
#include <vector>


namespace CAS {
    struct Error {

        //An enum to hold error types
        static enum Errtype_t {
            //error types
        }

        Error(const std::string output, Error_t type, const unsigned long n_line, const unsigned short n_char);
        std::string output{""};
        CAS::Error::Errtype_t type;
        unsigned long n_line{0};
        unsigned short n_char{0};//character number in line where error starts
    }
    typedef(CAS::Error::Errtype_t) Error_t; //allows easy use of the error type

    class Error_Manager {
    public:

        //default constructor
        Error_Manager();

        //constructor
        Error_Manager(CAS::Error_Manager original);

        //adds an error to the error list
        void add_error(CAS::Error);

        unsigned int count_errs(const CAS::Error_t);

    private:
        //Vector holding the Errors for the current user input
        std::vector<CAS::Error> error_list;
    }

}
