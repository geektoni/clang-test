//
// Created by uriel on 26/09/16.
//

#ifndef CLING_STACKTRACECHACTION_H
#define CLING_STACKTRACECHACTION_H

#include "CHAction.h"

/// \brief Print the stacktrace to a temporary file
class StackTraceCHAction : public CHAction  {

public:
    StackTraceCHAction(llvm::SmallString<128> description) {this->description = description;}
    virtual void run(CHResult_t * res);

};

#endif //CLING_STACKTRACECHACTION_H
