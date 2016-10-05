//
// Created by uriel on 24/09/16.
//

#ifndef CLING_CRASHHANDLER_H
#define CLING_CRASHHANDLER_H

#include <system_error>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallString.h>
#include "CHAction.h"

namespace cling {

    /// \brief Main Crash Handler that manages the CHActions chains
    /// and execute all run methods.
    class CrashHandler {
    private:
        std::vector<CHAction*> actions;
        CHResult_t * res = new CHResult_t;

    public:
        CrashHandler();

        /// \brief Execute the CHAction chain
        void execute();

        /// \brief Add a new CHAction chain
        void makeChain(std::vector<CHAction*> actions);

        /// \brief Dump the chain description
        void dumpChain();

    };

    /// \brief Helper function to initialize everything
    void handleCrashSignalWrapper(void*);
}


#endif //CLING_CRASHHANDLER_H
