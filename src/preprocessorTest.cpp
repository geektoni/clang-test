//
// Test the clang preprocessor.
// Created by uriel on 10/07/16.
//
#include "clang/Lex/ModuleLoader.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/HeaderSearchOptions.h"
#include "clang/Basic/LLVM.h"
#include "clang/Basic/TargetInfo.h"

using namespace clang;

int main(int argc, char ** argv) {

    // Preprocessor Options
    PreprocessorOptions * PPOptions = new PreprocessorOptions();
    llvm::IntrusiveRefCntPtr<PreprocessorOptions> PPOpts = PPOptions;

    // Diagnostic Engine
    llvm::IntrusiveRefCntPtr<DiagnosticIDs> Diags = new DiagnosticIDs();
    DiagnosticOptions * DiagOpts = new DiagnosticOptions();
    DiagnosticsEngine * diags = new DiagnosticsEngine(Diags, DiagOpts);

    // Language options
    LangOptions * opts = new LangOptions();

    // Source file manager
    FileSystemOptions optionsFilesystem;
    SourceManager * SM = new SourceManager(*diags, *(new FileManager(optionsFilesystem)));

    // Header search
    llvm::IntrusiveRefCntPtr<HeaderSearchOptions> HeaderOpts = new HeaderSearchOptions();
    TargetOptions * systemTriple = new TargetOptions();
    systemTriple->Triple = llvm::sys::getDefaultTargetTriple();
    std::shared_ptr<TargetOptions> triple (systemTriple);
    TargetInfo * target = TargetInfo::CreateTargetInfo(*diags, triple);
    HeaderSearch * Headers = new HeaderSearch(HeaderOpts, *SM, *diags, *opts, target);

    // Module loader
    ModuleLoader * loader;

    // Create the preprocessor
    clang::Preprocessor * preprocessor = new clang::Preprocessor(PPOpts, *diags, *opts, *SM, *Headers, *loader);

    return 0;
}