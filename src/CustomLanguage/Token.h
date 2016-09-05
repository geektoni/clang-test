//
// Created by uriel on 05/09/16.
//

#ifndef CLANG_TEST_TOKEN_H
#define CLANG_TEST_TOKEN_H

#include <cstring>

// Possible token types
// TODO: Add value type (for example, int a instead of only a)
enum TokType {
    tok_eof = -1,
    tok_def = -2,
    tok_extern = -3,
    tok_identifier = -4,
    tok_number = -5,
    tok_undef_char = -6
};

// Class that define a Token
class Token {

private:
    // Token type
    TokType Type;
    // Token value
    std::string Value;

public:
    Token();
    Token(TokType type, std::string value) : Type(type), Value(value) {};
    Type getType() {return Type;}
    std::string getValue() {return Value;}
    void setType(TokType t) {Type = t;}
    void setValue(std::string v) {Value = v;}
};

#endif //CLANG_TEST_TOKEN_H
