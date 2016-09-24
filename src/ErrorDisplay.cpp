//
// Created by uriel on 22/09/16.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>
#include "BugReporter/CrashReporter.h"



int main(int argc, char ** argv) {

  void (*PS)(void*) = llvm::handleCrashSignalWrapper;
  llvm::sys::AddSignalHandler(PS, nullptr);

  // Generate a segmentation fault.
  int array[1] = {0};
  int a = array[2000000];
  // Pretty stupid? Yeah, you got that right man.

  return 0;
}