//
// Created by uriel on 26/08/16.
//

// RUN: ./../build/tokenPrinter %s

int a=3;

// CHECK: int 'int'
// CHECK: identifier 'a'
// CHECK: equal '='
// CHECK: numeric_constant '3'
// CHECK: semi ';'
// CHECK: eof ''

