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

  // Supondo que a tabela 'user' tem colunas como 'id', 'name', 'age'
  char *sql = "INSERT INTO user (username, password, name, avatar_url) VALUES ('kelvinseibt', '12345', 'Kelvin Isael Seibt', \"https://images.unsplash.com/photo-1494790108377-be9c29b29330?q=80&w=1287&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D\");";

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "[2] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  }

  sqlite3_close(db);
  return 0;
}
