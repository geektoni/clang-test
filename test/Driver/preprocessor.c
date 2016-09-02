//
// Check if the preprocessor work.
//
// RUN: ../../build/driver -E %s | grep 'int'

int a=3;

// CHECK: int a=3;
