#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

int main(int argc, char *argv[]) {
  sqlite3 *db;
  char *errMessage = 0;
  int rc;

  rc = sqlite3_open("src/db/note.db", &db);
  if (rc) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
  }
  
  char *noteTableSQL = "CREATE TABLE IF NOT EXISTS note ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "user_id INTEGER,"
    "title TEXT,"
    "content TEXT,"
    "updated_at DATETIME,"
    "created_at DATETIME,"
    "FOREIGN KEY (user_id) REFERENCES user(id));";

  rc = sqlite3_exec(db, noteTableSQL, callback, 0, &errMessage);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error running note table creation: %s\n", errMessage);
    sqlite3_free(errMessage);
  }

  sqlite3_close(db);
  
  return 0;
}