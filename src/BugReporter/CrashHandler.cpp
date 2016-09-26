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
}

void CrashHandler::makeChain(std::vector<CHAction*> act) {
  actions = act;
}

void CrashHandler::dumpChain() {
  for (unsigned int i=0; i<actions.size(); i++) {
    fprintf(stderr,"{%i}: %s\n", i, actions[i]->dump());
  }
}

void llvm::handleCrashSignalWrapper(void*){
  llvm::CrashHandler CR;

  std::vector<CHAction*> actions;
  actions.push_back(new PrintDiagnosticCHAction("Print diagnostic information"));
  actions.push_back(new StackTraceCHAction("Print stacktrace to file"));

  CR.makeChain(actions);
  CR.execute();
  CR.dumpChain();
  exit(1);
}