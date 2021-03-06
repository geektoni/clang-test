//
// Created by uriel on 13/07/16.
//
#include "clang/Lex/ModuleLoader.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/HeaderSearchOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Parse/ParseAST.h"
#include <iostream>
#include <clang/AST/ASTContext.h>
#include "clang/Sema/Sema.h"

#include "../lib/PrintPPOutputPPCallbacks.h"


using namespace clang;

int main(int argc, char ** argv) {

    // Preprocessor Options
    llvm::IntrusiveRefCntPtr<PreprocessorOptions> PPOpts(new PreprocessorOptions);

    // Diagnostic Engine
    llvm::IntrusiveRefCntPtr<DiagnosticIDs> Diags = new DiagnosticIDs();
    DiagnosticOptions * DiagOpts = new DiagnosticOptions();
    clang::TextDiagnosticPrinter *pTextDiagnosticPrinter = // Diagnostic Consumer
            new clang::TextDiagnosticPrinter(llvm::outs(), DiagOpts);
    DiagnosticsEngine * diags = new DiagnosticsEngine(Diags, DiagOpts, pTextDiagnosticPrinter);

    // Language options
    LangOptions * opts = new LangOptions();
    opts->CPlusPlus = 1;

    // Source file manager
    FileSystemOptions * optionsFilesystem = new FileSystemOptions();
    FileManager * FM = new FileManager(*optionsFilesystem);
    SourceManager * SM = new SourceManager(*diags, *FM);

    // Header search initialization and add header path
    llvm::IntrusiveRefCntPtr<HeaderSearchOptions> HeaderOpts = new HeaderSearchOptions();

    TargetOptions * systemTriple = new TargetOptions();
    systemTriple->Triple = llvm::sys::getDefaultTargetTriple();
    std::shared_ptr<TargetOptions> triple (systemTriple);
    TargetInfo * target = TargetInfo::CreateTargetInfo(*diags, triple);

    // These are system dependent. They works under Ubuntu 16.04
    HeaderSearch * Headers = new HeaderSearch(HeaderOpts, *SM, *diags, *opts, target);
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

    clang::ApplyHeaderSearchOptions(*Headers,
                                    Headers->getHeaderSearchOpts(),
                                    *opts,
                                    target->getTriple());

    // Module loader
    CompilerInstance * loader = new CompilerInstance();

    // Create the preprocessor
    clang::Preprocessor * preprocessor = new clang::Preprocessor(PPOpts, *diags, *opts, *SM, *Headers, *loader);

    preprocessor->Initialize(*target);
    clang::FrontendOptions frontendOptions;
    clang::InitializePreprocessor(
            *preprocessor,
            *PPOpts,
            *(new RawPCHContainerReader()),
            frontendOptions);


    // Get the file, open it and set up preprocessor
    if (argc <= 1) {
      std::perror("Missing file to parse");
      return 1;
    }

    const FileEntry * pFile = FM->getFile(argv[1]);
    SourceLocation * SL = new SourceLocation();
    SM->setMainFileID(
            SM->createFileID( pFile, *SL, clang::SrcMgr::C_User)
    );

    // Set up diagnostic, set up PPOptions and run the preprocessor
    pTextDiagnosticPrinter->BeginSourceFile(*opts, preprocessor);
    PreprocessorOutputOptions * PPOOpts = new PreprocessorOutputOptions();
    PPOOpts->ShowCPP=1;
    DoPrintPreprocessedInput(*preprocessor, &llvm::outs(), *PPOOpts);
    pTextDiagnosticPrinter->EndSourceFile();

    return 0;
}