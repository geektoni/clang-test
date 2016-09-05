//
// Created by uriel on 05/09/16.
//

#include "Parser.h"
#include "Token.h"

// Private methods //

void Parser::initializeLexer() {
  this->lexer = new Lexer();
}

/*********************/

std::unique_ptr<ExprAST> Parser::ParseNumberExpr(Token token) {
  auto result = llvm::make_unique<NumberExprAST>(strtod(
          token.getValue().c_str(), 0));
  lexer->getNextToken();
  return std::move(result);
}

std::unique_prt<ExprAST> Parser::ParseParenExpr() {
  lexer.getNextToken();
  auto V = ParseExpression();
  if (!V)
    return nullptr;
  if (lexer->getBufferedToken().getValue() != ')') {
    return LogError("expected ')'");
  }
  lexer.getNextToken();
  return V;
}

std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr(Token token) {
  td::string IdName = token.getValue();

  lexer->getNextToken();  // eat identifier.

  if (lexer->getBufferedToken().getValue() != '(') // Simple variable ref.
    return llvm::make_unique<VariableExprAST>(IdName);

  // Call.
  lexer->getNextToken();  // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (lexer->getBufferedToken().getValue() != ')') {
    while (1) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (lexer->getBufferedToken().getValue() == ')')
        break;

      if (lexer->getBufferedToken().getValue() != ',')
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
    case tok_undef_char:
      if (token.getValue() == '(')
        return ParseParenExpr();
      break;
    default:
      return LogError("Unknown token when expecting an expression");
  }
}

std::unique_ptr<ExprAST> Parser::ParseExpression(Token token) {
  auto LHS = ParsePrimary(token);
  if (!LHS)
    return nullptr;
  return ParseBinOpRHS(0, std::move(LHS), token);
}

std::unique_ptr<PrototypeAST> Parser::ParsePrototype(Token token) {
  if (lexer->getBufferedToken().getType() != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = token.getValue();
  lexer->getNextToken();

  if (lexer->getBufferedToken().getValue() != '(')
    return LogErrorP("Expected '(' in prototype");

  // Read the list of argument names.
  std::vector<std::string> ArgNames;
  lexer->getNextToken();

  while (lexer->getBufferedToken().getType() == tok_identifier)
    ArgNames.push_back(lexer->getBufferedToken().getValue());
  if (lexer->getBufferedToken().getValue() != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  getNextToken();  // eat ')'.

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
  return ParsePrototype();
}

std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = llvm::make_unique<PrototypeAST>("", std::vector<std::string>());
    return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

/*std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec,
                                               std::unique_ptr<ExprAST> LHS,
                                               Token token) {
  while(1) {
    int TokPrec = getTokPrecedence(token);
    if (TokPrec < ExprPrec)
      return LHS;

    auto RHS = ParsePrimary(getNextToken());
    if (!RHS)
      return nullptr;

    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS = helper::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                             std::move(RHS));
  }
}*/

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