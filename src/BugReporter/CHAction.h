//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_CHACTION_H
#define CLANG_TEST_CHACTION_H

#include <llvm/ADT/SmallString.h>
using namespace llvm;
typedef std::vector<SmallString<128>> CHResult;

class CHAction {

public:
    virtual void run(CHResult * res);
    virtual char * dump() {return description;}

protected:
    char * description;

};


#endif //CLANG_TEST_CHACTION_H
