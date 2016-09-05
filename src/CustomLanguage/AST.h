//
// Created by uriel on 05/09/16.
// Basic AST definition
//

#ifndef CLANG_TEST_AST_H
#define CLANG_TEST_AST_H

// Basic AST class. Everything else inherit from here
class ExprAST {
public:
    virtual ~ExprAST() {}
};

// Class for represent numeric literal
class NumberExprAST : public ExprAST {
    double Value;
public:
    NumberExprAST(double v) : Value(v) {}
};

// Class for variable declaration
class VariableExprAST : public  ExprAST {
    std::string Name;
public:
    VariableExprAST (const std::string &n) : Name(n) {}
};

// Class for binary expression
class BinaryExprAST : public ExprAST {
    char operation;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs,
                  std::unique_ptr<ExprAST> rhs)
            : operation(op), LHS(std::move(lhs)), RHS(std::move(rhs)) {}
};

// Class for function calls
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_pointer<ExprAST>> Args;
public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}
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
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
};


#endif //CLANG_TEST_AST_H
