//
// Created by uriel on 26/08/16.
//

// This should be a better implementation of the
// preprocessor using a CompilerInstance object.

#include <clang/Basic/TargetInfo.h>
#include "clang/Frontend/CompilerInstance.h"
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"

#include <iostream>

using namespace clang;

int main (int argc, char ** argv) {

  DiagnosticOptions * DiagOpts = new DiagnosticOptions();
  DiagOpts->Pedantic = 1;
  TextDiagnosticPrinter *pTextDiagnosticPrinter = // Diagnostic Consumer
          new TextDiagnosticPrinter(llvm::outs(), DiagOpts);

  CompilerInstance * CI = new CompilerInstance();

  CI->createDiagnostics(pTextDiagnosticPrinter);
  CI->createFileManager();
  CI->createSourceManager(CI->getFileManager());

  std::shared_ptr<TargetOptions> pTargetOpts (new TargetOptions());
  pTargetOpts.get()->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo * pTarget = TargetInfo::CreateTargetInfo(CI->getDiagnostics(), pTargetOpts);
  CI->setTarget(pTarget);

  CI->createPreprocessor(TU_Complete);

  // Header search initialization and add header path
  // These are system dependent. They works under Ubuntu 16.04
  HeaderSearch * Headers = &CI->getPreprocessor().getHeaderSearchInfo();

  Headers->getHeaderSearchOpts().AddPath("/usr/include/c++/4.9",
                                         clang::frontend::Angled,
                                         false,
                                         false);
  Headers->getHeaderSearchOpts().AddPath("/usr/include/c++/4.9/tr1",
                                         clang::frontend::Angled,
                                         false,
                                         false);
  Headers->getHeaderSearchOpts().AddPath("/usr/include/linux",
                                         clang::frontend::Angled,
                                         false,
                                         false);
  Headers->getHeaderSearchOpts().AddPath("/usr/include/x86_64-linux-gnu/c++/4.9",
                                         clang::frontend::Angled,
                                         false,
                                         false);
  Headers->getHeaderSearchOpts().AddPath("/usr/include/x86_64-linux-gnu/",
                                         clang::frontend::Angled,
                                         false,
                                         false);
  Headers->getHeaderSearchOpts().AddPath("/usr/include",
                                         clang::frontend::Angled,
                                         false,
                                         false);

  ApplyHeaderSearchOptions(*Headers,
                                  Headers->getHeaderSearchOpts(),
                                  CI->getLangOpts(),
                                  pTarget->getTriple());

  // Language options
  LangOptions * pLangOpts = new LangOptions();
  pLangOpts->CPlusPlus = 1;

  // File that will be set as main file of the CI. This file is
  // expected to be a C++ file.
  FrontendInputFile * pFile = new FrontendInputFile(argv[1], IK_CXX);
  CI->InitializeSourceManager(*pFile);

  // Lex the file and print out tokens from it.
  pTextDiagnosticPrinter->BeginSourceFile(*pLangOpts, &CI->getPreprocessor());
  CI->getPreprocessor().EnterMainSourceFile();
  pTextDiagnosticPrinter->EndSourceFile();




  return 0;
}