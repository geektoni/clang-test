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
#include <iostream>

using namespace clang;

int main(int argc, char ** argv) {

    // Preprocessor Options
    PreprocessorOptions * PPOptions = new PreprocessorOptions();
    llvm::IntrusiveRefCntPtr<PreprocessorOptions> PPOpts = PPOptions;

    // Diagnostic Engine
    llvm::IntrusiveRefCntPtr<DiagnosticIDs> Diags = new DiagnosticIDs();
    DiagnosticOptions * DiagOpts = new DiagnosticOptions();
    clang::TextDiagnosticPrinter *pTextDiagnosticPrinter = // Diagnostic Consumer
            new clang::TextDiagnosticPrinter(llvm::outs(), DiagOpts);
    DiagnosticsEngine * diags = new DiagnosticsEngine(Diags, DiagOpts, pTextDiagnosticPrinter);

    // Language options
    LangOptions * opts = new LangOptions();

    // Source file manager
    FileSystemOptions * optionsFilesystem = new FileSystemOptions();
    FileManager * FM = new FileManager(*optionsFilesystem);
    SourceManager * SM = new SourceManager(*diags, *FM);

    // Header search
    llvm::IntrusiveRefCntPtr<HeaderSearchOptions> HeaderOpts = new HeaderSearchOptions();
    TargetOptions * systemTriple = new TargetOptions();
    systemTriple->Triple = llvm::sys::getDefaultTargetTriple();
    std::shared_ptr<TargetOptions> triple (systemTriple);
    TargetInfo * target = TargetInfo::CreateTargetInfo(*diags, triple);
    HeaderSearch * Headers = new HeaderSearch(HeaderOpts, *SM, *diags, *opts, target);

    // Module loader
    CompilerInstance * loader = new CompilerInstance();

    // Create the preprocessor
    clang::Preprocessor * preprocessor = new clang::Preprocessor(PPOpts, *diags, *opts, *SM, *Headers, *loader);


    // Get the file, open it and set up preprocessor
    const FileEntry * pFile = FM->getFile(argv[1]);
    SourceLocation * SL = new SourceLocation();
    SM->setMainFileID(
            SM->createFileID( pFile, *SL, clang::SrcMgr::C_User)
    );
    preprocessor->EnterMainSourceFile();

    // Tokenize and dump tokens to stdout
    Token * token = new Token();
    do {
        preprocessor->Lex(*token);
        if(diags->hasErrorOccurred())
        {
            break;
        }
        preprocessor->DumpToken(*token);
        std::cerr << std::endl;
    } while(token->isNot(clang::tok::eof));

    return 0;
}