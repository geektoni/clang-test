//
// Created by uriel on 05/09/16.
//

#include <memory>
#include <llvm/ADT/STLExtras.h>
#include "Parser.h"

// Private methods //

void Parser::initializeLexer() {
  this->lexer = new Lexer();
}

/*********************/

void Parser::parse() {
  while(1) {
    switch(getTokenFromLexer().getType()) {
      case tok_eof:
        return;
      case tok_semicolon:
        fprintf(stderr, "ready> ");
        getLexer()->getNextToken();
        break;
      case tok_def:
        if (auto FnAST = ParseDefinition()) {
          if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:");
            FnIR->dump();
          }
        } else {
          // Skip token for error recovery.
          getLexer()->getNextToken();
        }
        break;
      case tok_extern:
        if (auto ExtAST = ParseExtern()) {
          if (auto *ExtIR = ExtAST->codegen()) {
            fprintf(stderr, "Parsed an extern\n");
            ExtIR->dump();
          }
        } else {
          // Skip token for error recovery.
          getLexer()->getNextToken();
        }
        break;
      case tok_identifier:
      case tok_number:
      case tok_left_bracket:
        if (auto TopAST = ParseTopLevelExpr()) {
          if (auto * TopIR = TopAST->codegen()) {
            fprintf(stderr, "Parsed a top-level expr\n");
            TopIR->dump();
          }
        } else {
          // Skip token for error recovery.
          getLexer()->getNextToken();
        }
        break;
      default:
        fprintf(stderr, "ready> ");
        getLexer()->getNextToken();
        break;
    }
  }
}

std::unique_ptr<ExprAST> Parser::ParseNumberExpr(Token token) {
  auto result = llvm::make_unique<NumberExprAST>(strtod(
          token.getValue().c_str(), 0));
  lexer->getNextToken();
  return std::move(result);
}

std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
  lexer->getNextToken();
  auto V = ParseExpression(lexer->getBufferedToken());
  if (!V)
    return nullptr;
  if (lexer->getBufferedToken().getType() != tok_right_bracket) {
    return LogError("expected ')'");
  }
  lexer->getNextToken();
  return V;
}

std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr(Token token) {
  std::string IdName = token.getValue();

  lexer->getNextToken();  // eat identifier.

  if (lexer->getBufferedToken().getType() != tok_left_bracket) // Simple variable ref.
    return llvm::make_unique<VariableExprAST>(IdName);

  // Call.
  lexer->getNextToken();  // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (lexer->getBufferedToken().getType() != tok_right_bracket) {
    while (1) {
      if (auto Arg = ParseExpression(lexer->getBufferedToken()))
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (lexer->getBufferedToken().getType() == tok_right_bracket)
        break;

      if (lexer->getBufferedToken().getValue() != ",")
        return LogError("Expected ')' or ',' in argument list");
      lexer->getNextToken();
    }
  }

  // Eat the ')'.
  lexer->getNextToken();

  return llvm::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> Parser::ParsePrimary(Token token) {
  switch (token.getType()) {
    case tok_identifier:
      return ParseIdentifierExpr(token);
    case tok_number:
      return ParseNumberExpr(token);
    case tok_left_bracket:
      return ParseParenExpr();
    default:
      return LogError("Unknown token when expecting an expression");
  }
  return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseExpression(Token token) {
  auto LHS = ParsePrimary(token);
  if (!LHS)
    return nullptr;
  return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<PrototypeAST> Parser::ParsePrototype(Token token) {
  if (lexer->getBufferedToken().getType() != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = token.getValue();
  lexer->getNextToken();

  if (lexer->getBufferedToken().getType() != tok_left_bracket)
    return LogErrorP("Expected '(' in prototype");

  // Read the list of argument names.
  std::vector<std::string> ArgNames;
  while (lexer->getNextToken().getType() == tok_identifier)
    ArgNames.push_back(lexer->getBufferedToken().getValue());
  if (lexer->getBufferedToken().getType() != tok_right_bracket)
    return LogErrorP("Expected ')' in prototype");

  // success.
  lexer->getNextToken();  // eat ')'.

  return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
  lexer->getNextToken();
  auto Proto = ParsePrototype(lexer->getBufferedToken());
  if (!Proto) return nullptr;

  if (auto E = ParseExpression(lexer->getBufferedToken())) {
    return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
  lexer->getNextToken();
  return ParsePrototype(lexer->getBufferedToken());
}

std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
  if (auto E = ParseExpression(lexer->getBufferedToken())) {
    // Make an anonymous proto.
    auto Proto = llvm::make_unique<PrototypeAST>("", std::vector<std::string>());
    return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec,
                                               std::unique_ptr<ExprAST> LHS) {
  while(1) {
    int TokPrec = getTokPrecedence();
    if (TokPrec < ExprPrec)
      return LHS;

    std::string BinOp = lexer->getBufferedToken().getValue();

    auto RHS = ParsePrimary(lexer->getNextToken());
    if (!RHS)
      return nullptr;

    int NextPrec = getTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS = llvm::make_unique<BinaryExprAST>(BinOp.c_str()[0], std::move(LHS),
                                             std::move(RHS));
  }
}

// Helper methods
int Parser::getTokPrecedence() {

  if (!isascii(lexer->getBufferedToken().getValue().c_str()[0]))
    return -1;

  int tokenPrecedence = this->BinaryOperationPrecedence[
          lexer->getBufferedToken().getValue().c_str()[0]];
  if (tokenPrecedence <=0) {
    return -1;
  }
  return tokenPrecedence;
}

void Parser::initializeBinOpPrec() {
  this->BinaryOperationPrecedence['>'] = 10;
  this->BinaryOperationPrecedence['+'] = 20;
  this->BinaryOperationPrecedence['-'] = 30;
  this->BinaryOperationPrecedence['*'] = 40;
}