//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_STACKTRACECHACTION_H
#define CLANG_TEST_STACKTRACECHACTION_H

#include "CHAction.h"

class StackTraceCHAction : public CHAction  {

public:
    StackTraceCHAction(char * description) {this->description = description;}
    virtual void run();

};


#endif //CLANG_TEST_STACKTRACECHACTION_H
