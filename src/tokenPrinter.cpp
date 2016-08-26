//
// Test the clang preprocessor.
// Created by uriel on 10/07/16.
//
#include "clang/Lex/ModuleLoader.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/HeaderSearchOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/LLVM.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Parse/ParseAST.h"
#include <iostream>
#include <clang/AST/ASTContext.h>
#include "clang/Sema/Sema.h"

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
    const FileEntry * pFile = FM->getFile(argv[1]);
    SourceLocation * SL = new SourceLocation();
    SM->setMainFileID(
            SM->createFileID( pFile, *SL, clang::SrcMgr::C_User)
    );

    preprocessor->EnterMainSourceFile();
    pTextDiagnosticPrinter->BeginSourceFile(*opts, preprocessor);

    // Tokenize and dump tokens to stdout
    Token * token = new Token();
    do {
        preprocessor->Lex(*token);
        if(diags->hasErrorOccurred()) {
            break;
        }

        // Print the token
        preprocessor->DumpToken(*token);
        std::cerr << std::endl;

    } while(token->isNot(clang::tok::eof));
    pTextDiagnosticPrinter->EndSourceFile();

    return 0;
}