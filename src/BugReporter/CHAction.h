//
// Created by uriel on 26/09/16.
//

#ifndef CLING_CHACTION_H
#define CLING_CHACTION_H

#include <llvm/ADT/SmallString.h>
#include <map>

typedef std::map<char, llvm::SmallString<128>> CHResult_t;
typedef std::pair<char, llvm::SmallString<128>> CHResultItem_t;

/// \brief Class that represent a generic Crash Handler Action.
///
/// This class can be used to build custom CHAction that will
/// be called if the program crash.
class CHAction {

public:

    /// \brief This method will be called by the crash handler. The
    /// action logic has to be put inside this method.
    /// \param res
    virtual void run(CHResult_t * res);

    /// \brief Return the description of this CHAction.
    /// \return
    virtual llvm::SmallString<128> dump() {return description;}

protected:
    llvm::SmallString<128> description;

};


#endif //CLING_CHACTION_H
