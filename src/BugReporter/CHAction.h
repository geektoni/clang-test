//
// Created by uriel on 26/09/16.
//

#ifndef CLANG_TEST_CHACTION_H
#define CLANG_TEST_CHACTION_H

#include <llvm/ADT/SmallString.h>
typedef std::vector<llvm::SmallString<128>> * Result;

class CHAction {

public:
    virtual void run(Result res);
    virtual char * dump() {return description;}

protected:
    char * description;

};


#endif //CLANG_TEST_CHACTION_H
