//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_STACKTRACECHACTION_H
#define CLANG_TEST_STACKTRACECHACTION_H

#include "CHAction.h"

class StackTraceCHAction : public CHAction  {

public:
    StackTraceCHAction(llvm::SmallString<128> description) {this->description = description;}
    virtual void run(CHResult_t * res);

};

#endif //CLANG_TEST_STACKTRACECHACTION_H
