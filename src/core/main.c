#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "./lib/dotenv.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("{ code: 400, message: \"Bad request\" }");
    return 1;
  }

  int method = atoi(argv[1]);

  switch(method) {
    case 0:
      /* Login
        No login há dois parametros: 
         - argv[2] username
         - argv[3] password

        Já estão no formato desejado [string], portanto é necessário somente 
        repassar os valores para a função.
      */
      //login(argv[2], argv[3]);
      break;
    case 1:
      /* Register
        No registro há tres parametros: 
         - argv[2] username
         - argv[3] password
         - argv[4] name

        Já estão no formato desejado [string], portanto é necessário somente 
        repassar os valores para a função.
      */
      //register(argv[2], argv[3], argv[4]);
      break;
    case 3:
      // Note creation
      printf ("Note creation\n");
      break;
    case 4:
      // Read all notes
      printf ("Read all notes\n");
      break;
    case 5:
      // Read a single note
      printf ("Read note\n");
      break;
    case 6:
      // Note update
      printf ("Update note\n");
      break;
    case 7:
      // Delete note
      printf ("Delete note\n");
      break;
    default:
      printf("{ code: 400, message: \"Bad request\" }");
  };

  return 0;
}