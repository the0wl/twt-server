#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "dotenv.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Bad Request", argv[0]);
    return 1;
  }

  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);
  int sum = num1 + num2;

  sqlite3 *db;
  char *errMessage = 0;
  int rc;

  rc = sqlite3_open("./src/db/note.db", &db);
  if (rc) {
    fprintf(stderr, "Não deu: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  char sql[100];
  sprintf(sql, "INSERT INTO sums (num1,num2,result) VALUES (%d, %d, %d);", num1, num2, sum);

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL ruim: %s\n", errMessage);
    sqlite3_free(errMessage);
  }

  sqlite3_close(db);
  printf("%d", sum);
  return 0;
}