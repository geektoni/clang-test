//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_CHACTION_H
#define CLANG_TEST_CHACTION_H

#include <llvm/ADT/SmallString.h>
typedef std::vector<llvm::SmallString<128>> CHResult;

class CHAction {

public:
    virtual void run(CHResult * res);
    virtual llvm::SmallString<128> dump() {return description;}

protected:
    llvm::SmallString<128> description;

};


#endif //CLANG_TEST_CHACTION_H
