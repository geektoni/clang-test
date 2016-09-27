//
// Created by uriel on 24/09/16.
//

#ifndef CLANG_TEST_CRASHREPORTER_H
#define CLANG_TEST_CRASHREPORTER_H

#include <system_error>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallString.h>
#include "CHAction.h"

namespace llvm {

    class CrashHandler {
    private:
        std::vector<CHAction*> actions;
        Result res = new std::vector<llvm::SmallString<128>>;

    public:
        CrashHandler();

        void execute();
        void makeChain(std::vector<CHAction*> actions);
        void dumpChain();

    };
    void handleCrashSignalWrapper(void*);
}


#endif //CLANG_TEST_CRASHREPORTER_H
