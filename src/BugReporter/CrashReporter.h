//
// Created by uriel on 24/09/16.
//

#ifndef CLANG_TEST_CRASHREPORTER_H
#define CLANG_TEST_CRASHREPORTER_H


#include <system_error>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallString.h>

namespace llvm {

    class CrashReporter {

    public:
        CrashReporter();

        void handleCrashSignal();
        void generateStackTraceFile(SmallString<128> &ResultPath);


    };
    void handleCrashSignalWrapper(void*);
}


#endif //CLANG_TEST_CRASHREPORTER_H
