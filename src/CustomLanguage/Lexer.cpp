//
// Created by uriel on 05/09/16.
//

#include "Lexer.h"
#include "Token.h"

// Get the next token from standard input
Token Lexer::getTok() {

  Token tmp;
  std::string tokenValue;

  // Jump whitespaces
  while(isspace(LastChar))
    LastChar = getchar();

  // Check if there are specific keyword
  if (isalpha(LastChar)) {
    tokenValue = LastChar;
    while (isalnum((LastChar = getchar())))
      tokenValue += LastChar;

    if (tokenValue == "def") {
      tmp.setType(tok_def);
    } else if(tokenValue == "extern") {
      tmp.setType(tok_extern);
    } else {
      tmp.setType(tok_identifier);
    }

    tmp.setValue(tokenValue);
    return tmp;
  }

  // Check if number
  if (isdigit(LastChar) || LastChar == '.') {
    do {
      tokenValue += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');
    tmp.setValue(tokenValue);
    tmp.setType(tok_number);
    return tmp;
  }

  // Skip comments TODO: Add multi-line comments
  if (LastChar == '#') {
    do {
      LastChar = getchar();
    } while (LastChar != EOF
             && LastChar != '\n'
             && LastChar != '\r');

    if (LastChar != EOF) {
      return getTok();
    }
  }

  // Check if EOF
  if (LastChar == EOF) {
    tmp.setValue('\0');
    tmp.setType(tok_eof);
    return tmp;
  }

  // Handle different values
  tokenValue = LastChar;
  tmp.setValue(tokenValue);
  tmp.setType(tok_undef_char);
  LastChar = getchar();

  return tmp;
}

// Implement a simple token buffer
Token Lexer::getNextToken() {
  return this->currentToken = getTok();
}