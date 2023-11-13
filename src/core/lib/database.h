#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
// #include "dotenv.h"

char **query(const char *rawSQL){
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("src/db/note.db", &db);

    if (rc)
    {
        fprintf(stderr, "Cannot open database: %s\\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, rawSQL, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot run query %s\\n\\n", rawSQL);
        return NULL;
    }

    // Array de strings para armazenar os registros
    char **registros = NULL;
    int num_registros = 0;

    // Executar a consulta e percorrer os resultados
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // Processar cada coluna do resultado
        int col_count = sqlite3_column_count(stmt);

        char *registro = (char *)malloc(256);

        if (registro == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para registro.");
            return NULL;
        }

        // Construir o registro como uma string
        registro[0] = '\0'; // Garantir que a string esteja vazia antes de adicionar dados

        for (int i = 0; i < col_count; i++)
        {
            const char *texto = (const char *)sqlite3_column_text(stmt, i);
            if (texto) {
                // Certifique-se de que há espaço suficiente antes de concatenar
                if (strlen(registro) + strlen(texto) < 255) {
                    strcat(registro, texto);
                } else {
                    // Não há espaço suficiente, precisamos realocar ou tratar o erro
                    fprintf(stderr, "Registro muito grande para o buffer.\n");
                    free(registro);
                    // Antes de retornar, deveríamos liberar os recursos já alocados
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return NULL;
                }
            }
            strcat(registro, ";");
        }

        // Adicionar o registro ao array de strings
        registros = (char **)realloc(registros, (num_registros + 1) * sizeof(char *));
        registros[num_registros] = registro;
        num_registros++;
    }

    // Adicionar um ponteiro NULL no final do array para indicar o final
    registros = (char **)realloc(registros, (num_registros + 1) * sizeof(char *));
    registros[num_registros] = NULL;

    // Finalizar a consulta e fechar o banco de dados
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return registros;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

long insertUser(char *name, char *username, char *password) {
  sqlite3 *db;
  char *errMessage = 0;
  int rc;
  long id = 0;

  rc = sqlite3_open("src/db/note.db", &db);
  
  if (rc) {
    fprintf(stderr, "[1] Error: %s\n", sqlite3_errmsg(db));
    return id;
  }

  char sql[200]; 
  sprintf(sql, "INSERT INTO user (username, password, name, avatar_url) VALUES ('%s', '%s', '%s', '');", username, password, name);

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "[2] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  } else {
    id = sqlite3_last_insert_rowid(db);
  }

  sqlite3_close(db);

  return id;
}

long insertNote(char *user_id, char *title, char *content) {
  sqlite3 *db;
  char *errMessage = 0;
  int rc;
  long id = 0;

  rc = sqlite3_open("src/db/note.db", &db);

  if (rc) {
    fprintf(stderr, "[1] Error: %s\n", sqlite3_errmsg(db));
    return id;
  }

  char sql[200]; 
  sprintf(sql, "INSERT INTO note (user_id, title, content, created_at, updated_at) VALUES (%s, '%s', '%s', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);", user_id, title, content);

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "[2] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  } else {
    id = sqlite3_last_insert_rowid(db);
  }

  sqlite3_close(db);

  return id;
}

long editNote(char *id, char *title, char *content) {
  sqlite3 *db;
  char *errMessage = 0;
  int rc;
  long noteUpdatedId = 0;

  rc = sqlite3_open("src/db/note.db", &db);

  if (rc) {
    fprintf(stderr, "[1] Error: %s\n", sqlite3_errmsg(db));
    return noteUpdatedId;
  }

  char sql[200];
  sprintf(sql, "UPDATE note SET title = '%s', content = '%s', updated_at = CURRENT_TIMESTAMP WHERE id = %s;", title, content, id);

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "[2] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  } else {
    noteUpdatedId = atoi(id);
  }

  sqlite3_close(db);

  return noteUpdatedId;
}

long deleteNote(char *id) {
  sqlite3 *db;
  char *errMessage = 0;
  int rc;
  long noteUpdatedId = 0;

  rc = sqlite3_open("src/db/note.db", &db);

  if (rc) {
    fprintf(stderr, "[1] Error: %s\n", sqlite3_errmsg(db));
    return noteUpdatedId;
  }

  char sql[200];
  sprintf(sql, "DELETE FROM note WHERE id = %s;", id);

  rc = sqlite3_exec(db, sql, callback, 0, &errMessage);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "[2] Error: %s\n", errMessage);
    sqlite3_free(errMessage);
  } else {
    noteUpdatedId = atoi(id);
  }

  sqlite3_close(db);

  return noteUpdatedId;
}