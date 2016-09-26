//
// Created by uriel on 26/09/16.
//

#include <cstdio>
#include "PrintDiagnosticCHAction.h"

void PrintDiagnosticCHAction::run() {
  fprintf(stderr, "A FATAL ERROR HAS OCCURRED\n"
          "Please, send this file below to http://somesite.com to report the error:\n");
}