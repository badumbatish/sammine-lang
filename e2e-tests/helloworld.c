
// RUN: cc %s -o %t
// RUN: %t | FileCheck %s
//
// CHECK: hello world
#include <stdio.h>

int main() {
  puts("hello world");
  return 0;
}
