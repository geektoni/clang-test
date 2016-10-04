//
// Created by uriel on 26/09/16.
//

#ifndef CLING_CHACTION_H
#define CLING_CHACTION_H

#include <llvm/ADT/SmallString.h>
#include <map>

typedef std::map<char, llvm::SmallString<128>> CHResult_t;
typedef std::pair<char, llvm::SmallString<128>> CHResultItem_t;

class CHAction {

public:
    virtual void run(CHResult_t * res);
    virtual llvm::SmallString<128> dump() {return description;}

protected:
    llvm::SmallString<128> description;

};


#endif //CLING_CHACTION_H
