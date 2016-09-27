//
// Created by uriel on 27/09/16.
//

#ifndef CLANG_TEST_RESULT_H
#define CLANG_TEST_RESULT_H

#include <llvm/ADT/SmallString.h>

class Result {
private:
    Result() {};
    static Result * inst;
public:
    static Result * instance();
    void set(std::vector<llvm::SmallString<128>> s) {value = s;}
    void add(llvm::SmallString<128> val) {value.push_back(val);}
    std::vector<llvm::SmallString<128>> get() {return value;}
protected:
    std::vector<llvm::SmallString<128>> value;

};


#endif //CLANG_TEST_RESULT_H
