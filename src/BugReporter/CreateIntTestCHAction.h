//
// Created by uriel on 06/10/16.
//

#ifndef CLANG_TEST_CREATEINTTESTCHACTION_H
#define CLANG_TEST_CREATEINTTESTCHACTION_H


#include "CHAction.h"

class CreateIntTestCHAction : public CHAction{
public:
    CreateIntTestCHAction(llvm::SmallString<128> description) {this->description = description;}
    virtual void run(CHResult_t * res);

};


#endif //CLANG_TEST_CREATEINTTESTCHACTION_H
