//
// Created by uriel on 26/09/16.
//

#include <cstdio>
#include <llvm/Support/raw_ostream.h>
#include "PrintDiagnosticCHAction.h"

void PrintDiagnosticCHAction::run(CHResult_t * res) {
  llvm::errs() << "A FATAL ERROR HAS OCCURRED\n"
          << "Please, send this file below to http://somesite.com to report the error:\n";

  for (CHResult_t::iterator i= res->begin(); i != res->end(); i++) {
    llvm::errs() << i->second.c_str() << "\n";
  }
}