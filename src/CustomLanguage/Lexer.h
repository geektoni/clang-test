//
// Created by uriel on 05/09/16.
// Lexer for the custom language
//

#ifndef CLANG_TEST_LEXER_H
#define CLANG_TEST_LEXER_H

#include "Token.h"

class Lexer {
private:
    Token currentToken;
public:
    Lexer();
    ~Lexer();
    Token getTok();
    Token getNextToken();
    Token getBufferedToken() { return buffer; }
};


#endif //CLANG_TEST_LEXER_H
