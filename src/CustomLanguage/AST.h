//
// Created by uriel on 05/09/16.
// Basic AST definition
//

#ifndef CLANG_TEST_AST_H
#define CLANG_TEST_AST_H

#include <memory>
#include <vector>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule(new llvm::Module("Context", TheContext));
static std::map<std::string, llvm::Value *> NamedValues;

// Basic AST class. Everything else inherit from here
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual llvm::Value *codegen() = 0;
};

// Class for represent numeric literal
class NumberExprAST : public ExprAST {
    double Value;
public:
    NumberExprAST(double v) : Value(v) {}
    virtual llvm::Value *codegen();
};

// Class for variable declaration
class VariableExprAST : public  ExprAST {
    std::string Name;
public:
    VariableExprAST (const std::string &n) : Name(n) {}
    virtual llvm::Value *codegen();
};

// Class for binary expression
class BinaryExprAST : public ExprAST {
    char operation;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs,
                  std::unique_ptr<ExprAST> rhs)
            : operation(op), LHS(std::move(lhs)), RHS(std::move(rhs)) {}
    virtual llvm::Value *codegen();
};

// Class for function calls
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}
    virtual llvm::Value *codegen();
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args)
            : Name(name), Args(std::move(Args)) {}
    virtual llvm::Function *codegen();
    std::string getName() {return Name;}
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
    virtual llvm::Function *codegen();
};


#endif //CLANG_TEST_AST_H
