#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "lib/database.h"
#include "lib/dotenv.h"

void user_login(char *username, char *password);
void user_recover(char *access_token);
void user_register(char *name, char *username, char *password);

void note_list(char *access_token, char *user_id);
void note_get(char *access_token, char *user_id, char *id);
void note_create(char *access_token, char *user_id, char *title, char *content);
void note_edit(char *access_token, char *user_id, char *id, char *title, char *content);
void note_delete(char *access_token, char *user_id, char *id);
void about();

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("{ code: 400, message: \"Bad request\" }");
    return 1;
  }
  
  int method = atoi(argv[1]);

  switch(method) {
    case 1:
      /* Login
        No login há dois parametros: 
         - argv[2] username
         - argv[3] password
      */
      user_login(argv[2], argv[3]);
      break;
    case 2:
      /* Reconexão 
        Na atualização há apenas um parametro: 
         - argv[2] access_token
      */
      user_recover(argv[2]);
      break;
    case 3:
      /* Registro de usuário
        No registro há tres parametros: 
         - argv[2] name
         - argv[3] username
         - argv[4] password
      */
      user_register(argv[2], argv[3], argv[4]);
      break;
    case 4:
      /* Registro de nota
        No cadastro de nota há quatro parametros:
         - argv[2] access_token
         - argv[3] user_id
         - argv[4] title
         - argv[5] content
      */
      note_create(argv[2], argv[3], argv[4], argv[5]);
      break;
    case 5:
      /* Ler todas notas de um usuário
        No cadastro de nota há dois parametros:
         - argv[2] access_token
         - argv[3] user_id
      */
      note_list(argv[2], argv[3]);
      break;
    case 6:
      /* Ler uma nota
        No cadastro de nota há tres parametros:
         - argv[2] access_token
         - argv[3] user_id
         - argv[4] id
      */
      note_get(argv[2], argv[3], argv[4]);
      break;
    case 7:
      /* Editar uma nota
        Na edição de nota há cinco parametros:
         - argv[2] access_token
         - argv[3] user_id
         - argv[4] id
         - argv[5] title
         - argv[6] content
      */
      note_edit(argv[2], argv[3], argv[4], argv[5], argv[6]);
      break;
    case 8:
      /* Deletar uma nota
        Na deleção de nota há tres parametros:
         - argv[2] access_token
         - argv[3] user_id
         - argv[4] id
      */
      note_delete(argv[2], argv[3], argv[4]);
      break;
    case 9:
      /* Imprimir sobre:
        - O que faz
        - Integrantes do grupo
        - Data de desenvolvimento
      */
      about();
      break;
    default:
      printf("{ code: 400, message: \"Bad request\" }");
  };

  return 0;
}

void user_login(char *username, char *password) {
  char userSQL[100];
  sprintf(userSQL, "SELECT id, username, name, avatar_url FROM user WHERE username = '%s' AND password = '%s';", username, password);

  char **userData = query(userSQL);

  if (userData[0] != NULL) {
    printf("%s", userData[0]);
  } else {
    printf("404");
  }
}

void user_recover(char *access_token) {
  char userSQL[100];
  sprintf(userSQL, "SELECT id, username, name, avatar_url FROM user WHERE id = %s;", access_token);

  char **userData = query(userSQL);

  if (userData[0] != NULL) {
    printf("%s, %s", userData[0], "");
  } else {
    printf("404");
  }
}

void user_register(char *name, char *username, char *password) {
  char findUserSQL[100];
  sprintf(findUserSQL, "SELECT id FROM user WHERE username = '%s';", username);

  char **findUserData = query(findUserSQL);

  if (findUserData[0] != NULL) {
    printf("401");
    return;
  }

  long userId = insertUser(name, username, password);

  if (userId > 0) {
    char userSQL[100];
    sprintf(userSQL, "SELECT id, username, name, avatar_url FROM user WHERE id = %ld;", userId);

    char **userData = query(userSQL);

    if (userData[0] != NULL) {
      printf("%s, %s", userData[0], "");
    } else {
      printf("404");
    }
  } else {
    printf("400");
  }
}

void note_list(char *access_token, char *user_id) {
  if (strcmp(access_token, user_id) != 0) {
    printf("401");
    return;
  }

  char noteSQL[100];
  sprintf(noteSQL, "SELECT id, title, content FROM note WHERE user_id = '%s';", user_id);

  char **noteData = query(noteSQL);
  int i = 0;

  if (noteData != NULL) {
    while(noteData[i] != NULL) {
      printf("%s\n", noteData[i]);
      i++;
    }
  }
}

void note_get(char *access_token, char *user_id, char *id) {
  if (strcmp(access_token, user_id) != 0) {
    printf("401");
    return;
  }

  char noteSQL[100];
  sprintf(noteSQL, "SELECT id, title, content, user_id FROM note WHERE id = %s;", id);

  char **noteData = query(noteSQL);

  if (noteData[0] != NULL) {
    printf("%s", noteData[0]);
  } else {
    printf("404");
  }
}

void note_create(char *access_token, char *user_id, char *title, char *content) {
  if (strcmp(access_token, user_id) != 0) {
    printf("401");
    return;
  }

  long noteId = insertNote(user_id, title, content);

  if (noteId > 0) {
    char noteSQL[100];
    sprintf(noteSQL, "SELECT id, title, content FROM note WHERE id = %ld;", noteId);

    char **noteData = query(noteSQL);

    if (noteData[0] != NULL) {
      printf("%s", noteData[0]);
    } else {
      printf("404");
    }
  } else {
    printf("400");
  }
}

void note_edit(char *access_token, char *user_id, char *id, char *title, char *content) {
  if (strcmp(access_token, user_id) != 0) {
    printf("401");
    return;
  }

  long noteId = editNote(user_id, title, content);

  if (noteId > 0) {
    char noteSQL[100];
    sprintf(noteSQL, "SELECT id, title, content FROM note WHERE id = %ld;", noteId);

    char **noteData = query(noteSQL);

    if (noteData[0] != NULL) {
      printf("%s", noteData[0]);
    } else {
      printf("404");
    }
  } else {
    printf("400");
  }
}

void note_delete(char *access_token, char *user_id, char *id) {
  if (strcmp(access_token, user_id) != 0) {
    printf("401");
    return;
  }

  long noteId = deleteNote(id);

  if (noteId > 0) {
    printf("201");
  } else {
    printf("400");
  }
}

void about() {
  printf("%s;%s;%s", "Kelvin Isael Seibt", "Aplicativo de anotações", "11/13/2023");
}