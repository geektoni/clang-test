//
// Created by uriel on 05/09/16.
//
#include <llvm/Support/raw_ostream.h>
#include "include/Token.h"
#include "include/Parser.h"
#include <llvm/Support/Signals.h>

int main() {

  llvm::sys::PrintStackTraceOnErrorSignal();

  Parser * P = new Parser();
  fprintf(stderr, "ready> ");
  P->getLexer()->getNextToken();

  P->parse();

  return 0;
}
