//
// Created by uriel on 26/09/16.
//

#include <cstdio>
#include <llvm/Support/raw_ostream.h>
#include "PrintDiagnosticCHAction.h"
#include "Result.h"

void PrintDiagnosticCHAction::run() {
  llvm::errs() << "A FATAL ERROR HAS OCCURRED\n"
          << "Please, send this file below to http://somesite.com to report the error:\n";
  for (unsigned int i=0; i < Result::instance()->get().size(); i++) {
    llvm::errs() << Result::instance()->get()[i].c_str() << "\n";
  }
}