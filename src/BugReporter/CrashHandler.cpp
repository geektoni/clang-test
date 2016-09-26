//
// Created by uriel on 24/09/16.
//


#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>
#include "CrashHandler.h"
#include "StackTraceCHAction.h"
#include "PrintDiagnosticCHAction.h"

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
  actions.push_back(new PrintDiagnosticCHAction);
  actions.push_back(new StackTraceCHAction);

  CR.makeChain(actions);
  CR.execute();
}