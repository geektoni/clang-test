//
// Created by uriel on 06/10/16.
//

#include <system_error>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "CreateIntTestCHAction.h"

void CreateIntTestCHAction::run(CHResult_t *res) {

  int FD = -1;
  int FS_ST = -1;
  std::error_code EC;
  llvm::SmallString<128> Path;
  llvm::Twine STPath = llvm::Twine(res->operator[]('c'));

  EC = llvm::sys::fs::createTemporaryFile("Test", "sh",FD, Path);
  if (EC) {
    llvm::errs() << "Error while creating interestingness test.\n";
    exit(1);
  }

  EC = llvm::sys::fs::openFileForRead(STPath, FS_ST);
  if (EC) {
    llvm::errs() << "Error while opening stacktrace file.\n";
    exit(1);
  }

  llvm::raw_fd_ostream tmp(FD, true);
  tmp << "#!/bin/bash\n";
  tmp << "/home/uriel/Projects/clang-test/errors ";
  tmp << "| grep ";

}