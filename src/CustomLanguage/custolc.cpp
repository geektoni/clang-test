//
// Created by uriel on 05/09/16.
//
#include "Token.h"
#include "Parser.h"

void MainLoop(Parser * parser) {
  while(1) {
    switch(parser->getTokenFromLexer().getType()) {
      case tok_eof:
        return;
      case tok_semicolon:
        fprintf(stderr, "ready> ");
        parser->getLexer()->getNextToken();
        break;
      case tok_def:
        if (parser->ParseDefinition()) {
          fprintf(stderr, "Parsed a function definition.\n");
        } else {
          // Skip token for error recovery.
          parser->getLexer()->getNextToken();
        }
        break;
      case tok_extern:
        if (parser->ParseExtern()) {
          fprintf(stderr, "Parsed an extern\n");
        } else {
          // Skip token for error recovery.
          parser->getLexer()->getNextToken();
        }
        break;
      case tok_identifier:
        if (parser->ParseTopLevelExpr()) {
          fprintf(stderr, "Parsed a top-level expr\n");
        } else {
          // Skip token for error recovery.
          parser->getLexer()->getNextToken();
        }
        break;
      default:
        fprintf(stderr, "ready> ");
        parser->getLexer()->getNextToken();
        break;
    }
  }
}

int main() {

  Parser * P = new Parser();
  fprintf(stderr, "ready> ");
  P->getLexer()->getNextToken();

  MainLoop(P);

  return 0;
}
