#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "dotenv.h"

*char selectById(*char table, int id) {
    sqlite3 *db;
    char *errMessage = 0;
    int rc;

    rc = sqlite3_open("./src/db/note.db", &db);
    
    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    char sql[100];
    sprintf(sql, "SELECT * FROM %s WHERE id = %d;", table, id);

    rc = sqlite3_exec(db, sql, callback, 0, &errMessage);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot run query %s\n\nError: %s\n", sql, errMessage);
        sqlite3_free(errMessage);
    }

    sqlite3_close(db);
    printf("%d", sum);
}

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