//
// Created by uriel on 24/09/16.
//


#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/FileSystem.h>
#include "CrashHandler.h"
#include "StackTraceCHAction.h"
#include "PrintDiagnosticCHAction.h"
#include "CreateIntTestCHAction.h"

using namespace cling;
using namespace llvm;

CrashHandler::CrashHandler() {}

void CrashHandler::execute() {
  for (unsigned int i=0; i<actions.size(); i++) {
    actions[i]->run(res);
  }
}

void CrashHandler::makeChain(std::vector<CHAction*> act) {
  actions = act;
}

void CrashHandler::dumpChain() {
  for (unsigned int i=0; i<actions.size(); i++) {
    llvm::errs() << i<< ": " << actions[i]->dump() << "\n";
  }
}

void cling::handleCrashSignalWrapper(void*){
  CrashHandler CR;

  std::vector<CHAction*> actions;
  actions.push_back(new StackTraceCHAction(SmallString<128>("Print stacktrace to file")));
  actions.push_back(new CreateIntTestCHAction(SmallString<128>("Generate interestingness test")));
  actions.push_back(new PrintDiagnosticCHAction(SmallString<128>("Print diagnostic information")));

  CR.makeChain(actions);
  CR.execute();
  //CR.dumpChain(); DEBUG
  exit(1);
}