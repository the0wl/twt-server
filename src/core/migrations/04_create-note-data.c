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

  rc = sqlite3_open("src/db/note.db", &db);
  if (rc) {
    fprintf(stderr, "[1] Error: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  char *sql = "INSERT INTO note (user_id, title, content, created_at, updated_at) VALUES (1, 'Primeira anotação', 'Texto da primeira anotação', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);";

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "[2] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  }

  char *newSQL = "INSERT INTO note (user_id, title, content, created_at, updated_at) VALUES (1, 'Segunda anotação', 'Texto da segunda anotação', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);";

  rc = sqlite3_exec(db, newSQL, callback, 0, &errMessage);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "[3] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  }

  sqlite3_close(db);
  return 0;
}
