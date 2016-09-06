//
// Created by uriel on 05/09/16.
//
#include "Token.h"
#include "Parser.h"

#include "llvm/Support/Signals.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/ManagedStatic.h"

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
        if (auto FnAST = parser->ParseDefinition()) {
          if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:");
            FnIR->dump();
          }
        } else {
          // Skip token for error recovery.
          parser->getLexer()->getNextToken();
        }
        break;
      case tok_extern:
        if (auto ExtAST = parser->ParseExtern()) {
          if (auto *ExtIR = ExtAST->codegen()) {
            fprintf(stderr, "Parsed an extern\n");
            ExtIR->dump();
          }
        } else {
          // Skip token for error recovery.
          parser->getLexer()->getNextToken();
        }
        break;
      case tok_identifier:
      case tok_number:
        if (auto TopAST = parser->ParseTopLevelExpr()) {
          if (auto * TopIR = TopAST->codegen()) {
            fprintf(stderr, "Parsed a top-level expr\n");
            TopIR->dump();
          }
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

  llvm::sys::PrintStackTraceOnErrorSignal();

  Parser * P = new Parser();
  fprintf(stderr, "ready> ");
  P->getLexer()->getNextToken();

  MainLoop(P);

  return 0;
}
