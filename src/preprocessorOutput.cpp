//
// Created by uriel on 13/07/16.
//

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
#include <cstdio>
#include <clang/AST/ASTContext.h>
#include "clang/Sema/Sema.h"

#include "../lib/PrintPPOutputPPCallbacks.h"


using namespace clang;

//===----------------------------------------------------------------------===//
// Preprocessed token printer
//===----------------------------------------------------------------------===//



static void PrintPreprocessedTokens(Preprocessor &PP, Token &Tok,
                                    PrintPPOutputPPCallbacks *Callbacks,
                                    raw_ostream &OS) {
    bool DropComments = PP.getLangOpts().TraditionalCPP &&
                        !PP.getCommentRetentionState();

    char Buffer[256];
    Token PrevPrevTok, PrevTok;
    PrevPrevTok.startToken();
    PrevTok.startToken();
    while (1) {
        if (Callbacks->hasEmittedDirectiveOnThisLine()) {
            Callbacks->startNewLineIfNeeded();
            Callbacks->MoveToLine(Tok.getLocation());
        }

        // If this token is at the start of a line, emit newlines if needed.
        if (Tok.isAtStartOfLine() && Callbacks->HandleFirstTokOnLine(Tok)) {
            // done.
        } else if (Tok.hasLeadingSpace() ||
                   // If we haven't emitted a token on this line yet, PrevTok isn't
                   // useful to look at and no concatenation could happen anyway.
                   (Callbacks->hasEmittedTokensOnThisLine() &&
                    // Don't print "-" next to "-", it would form "--".
                    Callbacks->AvoidConcat(PrevPrevTok, PrevTok, Tok))) {
            OS << ' ';
        }

        if (DropComments && Tok.is(tok::comment)) {
            // Skip comments. Normally the preprocessor does not generate
            // tok::comment nodes at all when not keeping comments, but under
            // -traditional-cpp the lexer keeps /all/ whitespace, including comments.
            SourceLocation StartLoc = Tok.getLocation();
            Callbacks->MoveToLine(StartLoc.getLocWithOffset(Tok.getLength()));
        } else if (Tok.is(tok::annot_module_include) ||
                   Tok.is(tok::annot_module_begin) ||
                   Tok.is(tok::annot_module_end)) {
            // PrintPPOutputPPCallbacks::InclusionDirective handles producing
            // appropriate output here. Ignore this token entirely.
            PP.Lex(Tok);
            continue;
        } else if (IdentifierInfo *II = Tok.getIdentifierInfo()) {
            OS << II->getName();
        } else if (Tok.isLiteral() && !Tok.needsCleaning() &&
                   Tok.getLiteralData()) {
            OS.write(Tok.getLiteralData(), Tok.getLength());
        } else if (Tok.getLength() < 256) {
            const char *TokPtr = Buffer;
            unsigned Len = PP.getSpelling(Tok, TokPtr);
            OS.write(TokPtr, Len);

            // Tokens that can contain embedded newlines need to adjust our current
            // line number.
            if (Tok.getKind() == tok::comment || Tok.getKind() == tok::unknown)
                Callbacks->HandleNewlinesInToken(TokPtr, Len);
        } else {
            std::string S = PP.getSpelling(Tok);
            OS.write(&S[0], S.size());

            // Tokens that can contain embedded newlines need to adjust our current
            // line number.
            if (Tok.getKind() == tok::comment || Tok.getKind() == tok::unknown)
                Callbacks->HandleNewlinesInToken(&S[0], S.size());
        }
        Callbacks->setEmittedTokensOnThisLine();

        if (Tok.is(tok::eof)) break;

        PrevPrevTok = PrevTok;
        PrevTok = Tok;
        PP.Lex(Tok);
    }
}

typedef std::pair<const IdentifierInfo *, MacroInfo *> id_macro_pair;
static int MacroIDCompare(const id_macro_pair *LHS, const id_macro_pair *RHS) {
    return LHS->first->getName().compare(RHS->first->getName());
}

static void DoPrintMacros(Preprocessor &PP, raw_ostream *OS) {
    // Ignore unknown pragmas.
    PP.IgnorePragmas();

    // -dM mode just scans and ignores all tokens in the files, then dumps out
    // the macro table at the end.
    PP.EnterMainSourceFile();

    Token Tok;
    do PP.Lex(Tok);
    while (Tok.isNot(tok::eof));

    SmallVector<id_macro_pair, 128> MacrosByID;
    for (Preprocessor::macro_iterator I = PP.macro_begin(), E = PP.macro_end();
         I != E; ++I) {
        auto *MD = I->second.getLatest();
        if (MD && MD->isDefined())
            MacrosByID.push_back(id_macro_pair(I->first, MD->getMacroInfo()));
    }
    llvm::array_pod_sort(MacrosByID.begin(), MacrosByID.end(), MacroIDCompare);

    for (unsigned i = 0, e = MacrosByID.size(); i != e; ++i) {
        MacroInfo &MI = *MacrosByID[i].second;
        // Ignore computed macros like __LINE__ and friends.
        if (MI.isBuiltinMacro()) continue;

        PrintMacroDefinition(*MacrosByID[i].first, MI, PP, *OS);
        *OS << '\n';
    }
}

/// DoPrintPreprocessedInput - This implements -E mode.
///
void clang::DoPrintPreprocessedInput(Preprocessor &PP, raw_ostream *OS,
                                     const PreprocessorOutputOptions &Opts) {
    // Show macros with no output is handled specially.
    if (!Opts.ShowCPP) {
        assert(Opts.ShowMacros && "Not yet implemented!");
        DoPrintMacros(PP, OS);
        return;
    }

    // Inform the preprocessor whether we want it to retain comments or not, due
    // to -C or -CC.
    PP.SetCommentRetentionState(Opts.ShowComments, Opts.ShowMacroComments);

    PrintPPOutputPPCallbacks *Callbacks =
            new PrintPPOutputPPCallbacks(PP, *OS, !Opts.ShowLineMarkers,
                                         Opts.ShowMacros);
    PP.AddPragmaHandler(new UnknownPragmaHandler("#pragma", Callbacks));
    PP.AddPragmaHandler("GCC", new UnknownPragmaHandler("#pragma GCC",Callbacks));
    PP.AddPragmaHandler("clang",
                        new UnknownPragmaHandler("#pragma clang", Callbacks));

    PP.addPPCallbacks(std::unique_ptr<PPCallbacks>(Callbacks));

    // After we have configured the preprocessor, enter the main file.
    PP.EnterMainSourceFile();

    // Consume all of the tokens that come from the predefines buffer.  Those
    // should not be emitted into the output and are guaranteed to be at the
    // start.
    const SourceManager &SourceMgr = PP.getSourceManager();
    Token Tok;
    do {
        PP.Lex(Tok);
        if (Tok.is(tok::eof) || !Tok.getLocation().isFileID())
            break;

        PresumedLoc PLoc = SourceMgr.getPresumedLoc(Tok.getLocation());
        if (PLoc.isInvalid())
            break;

        if (strcmp(PLoc.getFilename(), "<built-in>"))
            break;
    } while (true);

    // Read all the preprocessed tokens, printing them out to the stream.
    PrintPreprocessedTokens(PP, Tok, Callbacks, *OS);
    *OS << '\n';
}


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

    pTextDiagnosticPrinter->BeginSourceFile(*opts, preprocessor);
    PreprocessorOutputOptions * PPOOpts = new PreprocessorOutputOptions();
    PPOOpts->ShowCPP=1;
    DoPrintPreprocessedInput(*preprocessor, &llvm::outs(), *PPOOpts);
    pTextDiagnosticPrinter->EndSourceFile();

    return 0;
}