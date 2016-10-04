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

    class CrashHandler {
    private:
        std::vector<CHAction*> actions;
        CHResult_t * res = new CHResult_t;

    public:
        CrashHandler();

        void execute();
        void makeChain(std::vector<CHAction*> actions);
        void dumpChain();

    };
    void handleCrashSignalWrapper(void*);
}


#endif //CLING_CRASHHANDLER_H
