//
// Created by uriel on 26/09/16.
//

#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "StackTraceCHAction.h"
#include "Result.h"

void StackTraceCHAction::run() {
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

  Result::instance()->add(Path);

}