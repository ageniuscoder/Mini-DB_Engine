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

// More specific exceptions (optional, for clarity)
class LexicalException : public CustomException {
public:
    explicit LexicalException(const std::string &message)
        : CustomException("Lexical Error: " + message) {}
};

class SyntaxException : public CustomException {
public:
    explicit SyntaxException(const std::string &message)
        : CustomException("Syntax Error: " + message) {}
};

class SemanticException : public CustomException {
public:
    explicit SemanticException(const std::string &message)
        : CustomException("Semantic Error: " + message) {}
};

class RuntimeDatabaseException : public CustomException {
public:
    explicit RuntimeDatabaseException(const std::string &message)
        : CustomException("Runtime Error: " + message) {}
};

#endif // !EXCEPTIONS_H