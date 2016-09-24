//
// Created by uriel on 24/09/16.
//

#include "CrashReporter.h"
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>

using namespace llvm;

CrashReporter::CrashReporter() {}

void CrashReporter::handleCrashSignal() {
  fprintf(stderr, "A FATAL ERROR HAS OCCURRED\n"
          "Please, send this file below to http://somesite.com to report the error:\n");

  llvm::SmallString<128> Path;
  generateStackTraceFile(Path);
  fprintf(stderr, Path.c_str());
  fprintf(stderr, "\n");

  exit(1);
}

void CrashReporter::generateStackTraceFile(SmallString<128> &ResultPath) {

  int FD = -1;
  std::error_code EC;
  llvm::SmallString<128> Path;

  EC = llvm::sys::fs::createTemporaryFile("StackTrace", "result",FD, Path);
  if (EC) {
    fprintf(stderr, "Error while creating StackTrace diagnostic file.");
    exit(1);
  }
  llvm::raw_fd_ostream tmp(FD, true);
  llvm::sys::PrintStackTrace(tmp);

  ResultPath = Path;
}


void llvm::handleCrashSignalWrapper(void*){
  llvm::CrashReporter CR;
  CR.handleCrashSignal();
}