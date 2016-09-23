//
// Created by uriel on 22/09/16.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>

void printStack(void*) {

  fprintf(stderr, "A FATAL ERROR HAS OCCURRED\n"
          "Please, send this file below to http://somesite.com to report the error:\n");

  int FD = -1;
  std::error_code EC;
  llvm::SmallString<128> Path;

  EC = llvm::sys::fs::createTemporaryFile("StackTrace", "result",FD, Path);
  if (EC) {
    fprintf(stderr, "Error while creating StackTrace diagnostic file.");
  }
  llvm::raw_fd_ostream tmp(FD, true);
  llvm::sys::PrintStackTrace(tmp);
  tmp.~raw_fd_ostream();

  fprintf(stderr, Path.c_str());
  fprintf(stderr, "\n");

  exit(1);
}

int main(int argc, char ** argv) {

  void (*PS)(void*) = printStack;
  llvm::sys::AddSignalHandler(PS, nullptr);

  // Generate a segmentation fault.
  int array[1] = {0};
  int a = array[2000000];
  // Pretty stupid? Yeah, you got that right man.

  return 0;
}