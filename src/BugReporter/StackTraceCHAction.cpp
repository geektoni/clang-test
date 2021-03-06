//
// Created by uriel on 26/09/16.
//

#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "StackTraceCHAction.h"

void StackTraceCHAction::run(CHResult_t * res) {
  int FD = -1;
  std::error_code EC;
  llvm::SmallString<128> Path;

  EC = llvm::sys::fs::createTemporaryFile("StackTrace", "result",FD, Path);
  if (EC) {
    llvm::errs() << "Error while creating StackTrace diagnostic file.\n";
    exit(1);
  }
  llvm::raw_fd_ostream tmp(FD, true);
  llvm::sys::PrintStackTrace(tmp);

  res->insert(CHResultItem_t('s', Path));

}