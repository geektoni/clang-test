//
// Created by uriel on 05/09/16.
//

#include "Lexer.h"

// Get the next token from standard input
Token Lexer::getTok() {

  Token tmp;
  std::string tokenValue;
  int LastChar = ' ';

  // Jump whitespaces
  while(isspace(LastChar))
    LastChar = getchar();

  // Check if there are specific keyword
  if (isalpha(LastChar)) {
    tokenValue = LastChar;
    while (isalnum((LastChar = getchar())))
      tokenValue += LastChar;

    switch (tokenValue){
      case "def":
        tmp.setType(tok_def);
        break;
      case "extern":
        tmp.setType(tok_extern);
        break;
      default:
        tmp.setType(tok_identifier);
      break;
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
  }

  // Skip comments TODO: Add multi-line comments
  if (LastChar == "#") {
    do {
      LastChar = getchar();
    } while (LastChar != EOF
             && LastChar != '\n'
             && LastChar != '\r');

    if (LastChar != EOF) {
      return gettok();
    }
  }

  // Check if EOF
  if (LastChar == EOF) {
    tmp.setValue(EOF);
    tmp.setType(tok_eof);
  }

  // Handle different values
  tmp.setValue(LastChar);
  tmp.setType(tok_undef_char);

  return Token;
}

// Implement a simple token buffer
Token Lexer::getNextToken() {
  return this->currentToken = getTok();
}