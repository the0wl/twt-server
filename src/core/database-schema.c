#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

int main() {
  sqlite3 *db;
  char *errMessage = 0;
  int rc;

  rc = sqlite3_open("./src/db/note.db", &db);
  if (rc) {
    fprintf(stderr, "Oops: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  char *sql = "CREATE TABLE sums (id INTEGER PRIMARY KEY AUTOINCREMENT, num1 INTEGER, num2 INTEGER, result INTEGER);";

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL, pls: %s\n", errMessage);
    sqlite3_free(errMessage);
  }

  sqlite3_close(db);
  printf("Tabela criada. ✅\n");
  return 0;
}