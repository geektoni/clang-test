//
// Created by uriel on 22/09/16.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>
#include "BugReporter/CrashHandler.h"



int main(int argc, char * argv[]) {

  if (strcmp("-d", argv[1])==0) {
    void (*PS)(void*) = cling::handleCrashSignalWrapper;
    llvm::sys::AddSignalHandler(PS, nullptr);
  }

  // Generate a segmentation fault.
  int array[1] = {0};
  int a = array[2000000];
  printf("%i", a); // Only to avoid compiler warning
  // Pretty stupid? Yeah, you got that right man.

  return 0;
}