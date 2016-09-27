//
// Created by uriel on 26/09/16.
//

#include <cstdio>
#include "PrintDiagnosticCHAction.h"
#include "Result.h"

void PrintDiagnosticCHAction::run() {
  fprintf(stderr, "A FATAL ERROR HAS OCCURRED\n"
          "Please, send this file below to http://somesite.com to report the error:\n");
  for (unsigned int i=0; i < Result::instance()->get().size(); i++) {
    fprintf(stderr, "%s\n", Result::instance()->get()[i].c_str());
  }
}