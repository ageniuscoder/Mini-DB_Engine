
#ifndef TYPES_H
#define TYPES_H

#include <string>

// Enumeration for different types of tokens in mini-SQL
enum class TokenType {
    UNKNOWN,        // Not recognized
    KEYWORD,        // e.g., SELECT, CREATE, TABLE
    IDENTIFIER,     // e.g., users, id, my_table
    INTEGER_LITERAL,// e.g., 1, 25, 100
    STRING_LITERAL, // e.g., 'mangal', 'lakshya'
    OPERATOR,       // e.g., =, >, <
    PUNCTUATOR,     // e.g., (, ), ,, ;
    END_OF_FILE     // Special marker for end of input
};

// A struct representing a single token
struct Token {
    TokenType type;     // The category of the token
    std::string value;  // The actual text value (e.g., "SELECT", "users")
};

#endif // TYPES_H
