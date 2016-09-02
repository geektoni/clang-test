//
// Created by uriel on 26/08/16.
//
// This should be a better implementation of the
// preprocessor using a CompilerInstance object.

#include "clang/AST/TypeLoc.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/FrontendTool/Utils.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/HeaderSearch.h"

#include <iostream>
#include <zconf.h>

using namespace clang;

int main (int argc, char ** argv) {

  // Generate a ostream to evade error reporting
  static llvm::raw_fd_ostream S(STDOUT_FILENO, false, true);

  CompilerInvocation * pInvocation = new CompilerInvocation();
  IntrusiveRefCntPtr<DiagnosticIDs> DiagsId (new DiagnosticIDs);
  DiagnosticOptions * DiagOpts = new DiagnosticOptions();
  clang::TextDiagnosticPrinter *DiagsCons = // Diagnostic Consumer
          new clang::TextDiagnosticPrinter(S, DiagOpts);
  DiagnosticsEngine *  Diags (new DiagnosticsEngine(DiagsId, DiagOpts, DiagsCons));


  std::vector<const char *> argDriver (argc-1);
  for (int i = 1; i < argc; ++i) {
    argDriver.push_back(argv[i]);
  }

  clang::CompilerInvocation::CreateFromArgs(*pInvocation, &(argDriver[0]),
                                            &(argDriver[argDriver.size()]),
                                            *(Diags));

  CompilerInstance *  CI2 (new CompilerInstance());
  CI2->setInvocation(pInvocation);
  CI2->setDiagnostics(Diags);

  clang::ExecuteCompilerInvocation(CI2);

  /*S.close();
  if (S.has_error()) {
    llvm::report_fatal_error("IO Error", false);

  }*/

  return 0;
}