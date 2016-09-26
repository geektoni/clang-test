//
// Created by uriel on 24/09/16.
//


#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>
#include "CrashHandler.h"
#include "StackTraceCHAction.h"

using namespace llvm;

CrashHandler::CrashHandler() {}

void CrashHandler::execute() {
  for (unsigned int i=0; i<actions.size(); i++) {
    actions[i]->run();
  }
  exit(1);
}

void CrashHandler::makeChain(std::vector<CHAction*> act) {
  actions = act;
}

void llvm::handleCrashSignalWrapper(void*){
  llvm::CrashHandler CR;

  std::vector<CHAction*> actions;
  actions.push_back(new StackTraceCHAction);

  CR.makeChain(actions);
  CR.execute();
}

/*void CrashHandler::handleCrashSignal() {
  fprintf(stderr, "A FATAL ERROR HAS OCCURRED\n"
          "Please, send this file below to http://somesite.com to report the error:\n");

  llvm::SmallString<128> Path;
  generateStackTraceFile(Path);
  fprintf(stderr, Path.c_str());
  fprintf(stderr, "\n");

  exit(1);
}*/