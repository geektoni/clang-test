//
// Created by uriel on 05/09/16.
//

#ifndef CLANG_TEST_PARSER_H
#define CLANG_TEST_PARSER_H

#include <map>
#include <memory>
#include "AST.h"
#include "Lexer.h"

/* TO DO LIST
 * TODO: Every statement must finish with a ;. If not it will cause a parser error;
 */


class Parser {
private:

    Lexer * lexer;

    // Binary operation precedence
    std::map<char, int> BinaryOperationPrecedence;

    // Diagnostic Methods
    std::unique_ptr<ExprAST> LogError(const char *message) {
      fprintf(stderr, "Parser error: %s\n", message);
      return nullptr;
    }
    std::unique_ptr<PrototypeAST> LogErrorP(const char * message) {
      LogError(message);
      return nullptr;
    }

    // Helper method
    void initializeLexer();
    void initializeBinOpPrec();

public:

    // Constructors
    Parser() {initializeLexer(); initializeBinOpPrec();}
    Parser(Lexer lexer) : lexer(&lexer) {initializeBinOpPrec();}

    // Main Loop
    void parse();

    // Parsing methods
    std::unique_ptr<ExprAST> ParseNumberExpr(Token token);
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParseIdentifierExpr(Token token);
    std::unique_ptr<ExprAST> ParsePrimary(Token token);
    std::unique_ptr<ExprAST> ParseExpression(Token token);
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                           std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<PrototypeAST> ParsePrototype(Token token);
    std::unique_ptr<FunctionAST> ParseDefinition();
    std::unique_ptr<PrototypeAST> ParseExtern();
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();

    // Helper methods
    int getTokPrecedence();
    Lexer * getLexer() {return lexer;}
    Token getTokenFromLexer() {return lexer->getBufferedToken();}
};


#endif //CLANG_TEST_PARSER_H
