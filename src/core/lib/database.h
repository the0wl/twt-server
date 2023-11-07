#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "dotenv.h"

char** selectById(char *table, int id) {    
    sqlite3 *db;
    char *errMessage = 0;
    int rc;

    rc = sqlite3_open("./src/db/note.db", &db);

    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    char sql[100];
    sprintf(sql, "SELECT * FROM %s WHERE id = %d;", table, id);

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot run query %s\n\nError: %s\n", sql, errMessage);
        sqlite3_free(errMessage);
    } else {
        // salvar dados sobre query em um arquivo
    }

    // Array de strings para armazenar os registros
    char **registros = NULL;
    int num_registros = 0;

    // Executar a consulta e percorrer os resultados
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Processar cada coluna do resultado
        int col_count = sqlite3_column_count(stmt);
        
        char *registro = (char *)malloc(256);
        
        if (registro == NULL) {
            fprintf(stderr, "Erro ao alocar memória para registro.");
            return NULL;
        }

        // Construir o registro como uma string
        registro[0] = '\0'; // Garantir que a string esteja vazia antes de adicionar dados
        
        for (int i = 0; i < col_count; i++) {
            strcat(registro, (const char *) sqlite3_column_text(stmt, i));
            strcat(registro, "\t"); // Adicionar um separador entre os valores
        }

        // Adicionar o registro ao array de strings
        registros = (char **)realloc(registros, (num_registros + 1) * sizeof(char *));
        registros[num_registros] = registro;
        num_registros++;
    }

    // Finalizar a consulta e fechar o banco de dados
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return registros;
}
