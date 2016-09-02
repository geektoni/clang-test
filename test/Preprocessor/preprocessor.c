//
// Check if the preprocessor work.
//
// RUN: ../../bin/preprocessor %s | grep 'int'

int a=3;

// CHECK: int a=3;
