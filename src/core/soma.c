#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);
  int sum = num1 + num2;

  printf("%d", sum);
  return 0;
}