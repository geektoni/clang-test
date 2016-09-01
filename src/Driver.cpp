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
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"
#include "clang/FrontendTool/Utils.h"

#include <iostream>
#include <cstring>
#include <clang/AST/TypeLoc.h>

using namespace clang;
/*
static void SetPreprocessorFromBinary(PreprocessorOptions& PPOpts) {
#ifdef _MSC_VER
  PPOpts.addMacroDef("_HAS_EXCEPTIONS=0");
#ifdef _DEBUG
    PPOpts.addMacroDef("_DEBUG=1");
#elif defined(NDEBUG)
    PPOpts.addMacroDef("NDEBUG=1");
#else // well, what else?
    PPOpts.addMacroDef("NDEBUG=1");
#endif
#endif

  // Since cling, uses clang instead, macros always sees __CLANG__ defined
  // In addition, clang also defined __GNUC__, we add the following two macros
  // to allow scripts, and more important, dictionary generation to know which
  // of the two is the underlying compiler.

#ifdef __clang__
  PPOpts.addMacroDef("__CLING__clang__=" ClingStringify(__clang__));
#elif defined(__GNUC__)
  //PPOpts.addMacroDef("__CLING__GNUC__=" ClingStringify(__GNUC__));
#endif

// https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html
#ifdef _GLIBCXX_USE_CXX11_ABI
  PPOpts.addMacroDef("_GLIBCXX_USE_CXX11_ABI="
                       ClingStringify(_GLIBCXX_USE_CXX11_ABI));
#endif
}*/


/// Set target-specific preprocessor defines.
static void SetPreprocessorFromTarget(PreprocessorOptions& PPOpts,
                                      const llvm::Triple& TTriple) {
  if (TTriple.getEnvironment() == llvm::Triple::Cygnus) {
    // clang "forgets" the basic arch part needed by winnt.h:
    if (TTriple.getArch() == llvm::Triple::x86) {
      PPOpts.addMacroDef("_X86_=1");
    } else if (TTriple.getArch() == llvm::Triple::x86_64) {
      PPOpts.addMacroDef("__x86_64=1");
    } else {
      llvm::errs() << "Warning in cling::CIFactory::createCI():\n"
              "unhandled target architecture "
                   << TTriple.getArchName() << '\n';
    }
  }
}

static bool SetupCompiler(CompilerInstance* CI, bool full) {
    // Set the language options, which cling needs.
    // This may have already been done via a precompiled header
    //if (full)
    //  SetClingCustomLangOpts(CI->getLangOpts());

    PreprocessorOptions& PPOpts = CI->getInvocation().getPreprocessorOpts();
    //SetPreprocessorFromBinary(PPOpts);

    PPOpts.addMacroDef("__CLING__");
    if (CI->getLangOpts().CPlusPlus11 == 1) {
      // http://llvm.org/bugs/show_bug.cgi?id=13530
      PPOpts.addMacroDef("__CLING__CXX11");
    }

    if (CI->getDiagnostics().hasErrorOccurred()) {
      llvm::errs() << "Compiler error to early in initialization.\n";
      return false;
    }

    CI->setTarget(TargetInfo::CreateTargetInfo(CI->getDiagnostics(),
                                               CI->getInvocation().TargetOpts));
    if (!CI->hasTarget()) {
      llvm::errs() << "Could not determine compiler target.\n";
      return false;
    }

    CI->getTarget().adjust(CI->getLangOpts());

    SetPreprocessorFromTarget(PPOpts, CI->getTarget().getTriple());
    return true;
}

int main (int argc, char ** argv) {

  using namespace driver;

  CompilerInvocation * pInvocation = new CompilerInvocation();
  StringRef * ClangExecutable = new StringRef("driver");
  StringRef * DefaultTargetTriple = new StringRef(llvm::sys::getDefaultTargetTriple());
  IntrusiveRefCntPtr<DiagnosticIDs> DiagsId (new DiagnosticIDs);
  DiagnosticConsumer * DiagsCons = new DiagnosticConsumer();
  DiagnosticOptions * DiagOpts = new DiagnosticOptions();
  std::unique_ptr<DiagnosticsEngine> Diags (new DiagnosticsEngine(DiagsId, DiagOpts, DiagsCons));


  std::vector<const char *> argDriver (argc-1);
  for (int i = 1; i < argc; ++i) {
    llvm::outs() << argv[i] << "\n";
    argDriver.push_back(argv[i]);
  }

  Driver * testDriver = new Driver(*ClangExecutable, *DefaultTargetTriple, *(Diags.get()));
  testDriver->setCheckInputsExist(false);
  ArrayRef<const char *> args(&(argDriver[0]), argDriver.size());
  std::unique_ptr<clang::driver::Compilation>
          Compilation(testDriver->BuildCompilation(args));

  clang::CompilerInvocation::CreateFromArgs(*pInvocation, &(argDriver[0]),
                                            &(argDriver[argDriver.size()-1]),
                                            *(Diags.get()));
  
  std::unique_ptr<CompilerInstance> CI2 (new CompilerInstance());
  CI2->setInvocation(pInvocation);
  CI2->setDiagnostics(Diags.get());

  SetupCompiler(CI2.get(), true);
  clang::ExecuteCompilerInvocation(CI2.get());

  return 0;
}