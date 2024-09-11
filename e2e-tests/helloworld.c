
// RUN: cc %s -o %t
// RUN: %t | grep -e "cat"
#include <stdio.h>

int main() {
  puts("hello world");
  return 0;
}
