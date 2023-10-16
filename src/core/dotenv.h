#ifndef ENV_LOADER_H
#define ENV_LOADER_H

void load_env(const char *file_path);
char *get_env_value(char *key);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 50
#define LINE_BUFFER_SIZE 256

typedef struct {
  char *key;
  char *value;
} EnvVar;

EnvVar *hashTable[TABLE_SIZE];

unsigned int hash(char *key) {
  unsigned int value = 0;
  while (*key) {
    value = (value << 8) + *key++;
  }
  return value % TABLE_SIZE;
}

void load_env(const char *file_path) {
  // Ler arquivo de ambiente ".env"
  FILE *file = fopen(file_path, "r");

  // Se não encontrar retorna mensagem e sai da função
  if (!file) {
    printf("No .env file");
    return;
  }

  // Aloca buffer para ler cada linha do arquivo.
  char line[LINE_BUFFER_SIZE];

  while (fgets(line, sizeof(line), file)) {
    // Dividir a linha em 2, com o caractere "="
    char *key = strtok(line, "=");
    // Dividir a linha em 2, com o caractere "\n".
    // Obs: NULL faz que *strtok* continue de onde parou da última vez.
    char *value = strtok(NULL, "\n");

    // Se encontrou valores
    if (key && value) {
      // Calcula o índice de hash para a chave
      unsigned int idx = hash(key);
      //Aloca memória para a variável de ambiente
      EnvVar *var = malloc(sizeof(EnvVar));
      // Copia a chave e o valor com *strdup*
      var->key = strdup(key);
      var->value = strdup(value);
      // Armazena no índice calculado no primeiro passo
      hashTable[idx] = var;
    }
  }

  fclose(file);
}

char *get_env_value(char *key) {
  // Calcula o índice de hash para a chave
  unsigned int idx = hash(key);

  // Se encontrar o indice na tabela
  if (hashTable[idx]) {
    // Retornar seu valor
    return hashTable[idx]->value;
  }

  // Se não encontrar retorne nulo
  return NULL;
}
