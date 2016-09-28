//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_PRINTDIAGNOSTICCHACTION_H
#define CLANG_TEST_PRINTDIAGNOSTICCHACTION_H

#include "CHAction.h"

class PrintDiagnosticCHAction : public CHAction {

public:
    PrintDiagnosticCHAction(char * description) {this->description = description;}
    virtual void run(CHResult * res);
};


#endif //CLANG_TEST_PRINTDIAGNOSTICCHACTION_H
