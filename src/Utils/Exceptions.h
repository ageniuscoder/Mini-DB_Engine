#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>
using namespace std;

// A custom exception class for our database system.
// This allows us to throw and catch specific errors
// with user-friendly messages.


class CustomException : public runtime_error{
    public:
    explicit CustomException(const string &message):runtime_error(message){}
};

#endif // !EXCEPTIONS_H