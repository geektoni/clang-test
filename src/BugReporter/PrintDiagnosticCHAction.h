//
// Created by uriel on 26/09/16.
//

#ifndef CLING_PRINTDIAGNOSTICCHACTION_H
#define CLING_PRINTDIAGNOSTICCHACTION_H

#include "CHAction.h"

/// \brief Prind diagnostic informations (stacktrace file, where to report the error etc.)
class PrintDiagnosticCHAction : public CHAction {

public:
    PrintDiagnosticCHAction(llvm::SmallString<128> description) {this->description = description;}
    virtual void run(CHResult_t * res);
};


#endif //CLING_PRINTDIAGNOSTICCHACTION_H
