#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "dotenv.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("{ code: 400, message: \"Bad request\" }");
    return 1;
  }

  int method = atoi(argv[1])

  switch(method) {
  case 0:
    // Login
    break;
  case 1:
    // Register
    break;
  case 3:
    // Note creation
    break;
  case 4:
    // Read all notes
    break;
  case 5:
    // Read a single note
    break;
  case 6:
    // Note update
    break;
  case 7:
    // Delete note
    break;
  default:
    printf("{ code: 400, message: \"Bad request\" }");
  }

  int num2 = atoi(argv[2]);
  int sum = num1 + num2;

  return 0;
}