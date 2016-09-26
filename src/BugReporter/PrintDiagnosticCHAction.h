//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_PRINTDIAGNOSTICCHACTION_H
#define CLANG_TEST_PRINTDIAGNOSTICCHACTION_H

#include "CHAction.h"

class PrintDiagnosticCHAction : public CHAction {

public:
    virtual void run();
};


#endif //CLANG_TEST_PRINTDIAGNOSTICCHACTION_H
