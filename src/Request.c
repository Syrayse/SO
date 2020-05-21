#include "Common.h"
#include "Request.h"
#include <stdio.h>

Request request_geneic(unsigned int ID, char **argv, unsigned int length) {
  unsigned int i;
  Request request = malloc(sizeof(struct request));

  request->ID = ID;
  request->nArgs = length;
  request->argv = malloc(sizeof(char*) * length);

  for(i = 0; i < length; i++) {
    request->argv[i] = strdup(argv[i]);
  }

  return request;
}

char* serialize_request(Request request, ssize_t *length) {
  int i;
  ssize_t byte_size = 0, used = 0;
  char * buffer = NULL;
  /* Assume que o argv termina com NULL*/
  unsigned int ID = request->ID;       /* Ocupa os primeiros 4 bytes */

  /* Se ha nArgs, há nArgs offsets */
  ssize_t *offsets = malloc(sizeof(ssize_t) * request->nArgs);
  unsigned int *lens = malloc(sizeof(unsigned int) * request->nArgs);

  /* Calcula os offsets primeiro */
  if(request->nArgs > 0) {
    offsets[0] = 0;

    for(i = 1; i < request->nArgs; i++) {
      lens[i - 1] = strlen(request->argv[i - 1]);
      byte_size += lens[i - 1];
      offsets[i] = byte_size;
    }

    lens[i - 1] = strlen(request->argv[i - 1]);
    byte_size += lens[i - 1];
  }

  /* Add overhead */
  byte_size += 2 * sizeof(unsigned int)
                  + request->nArgs * sizeof(ssize_t);

  /* Já pode alocar o tamanho do buffer */
  buffer = malloc(sizeof(char) * byte_size);

  /* Insere ID e Numero de Argumentos */
  used = sizeof(unsigned int);
  memcpy(buffer, &ID, used);
  memcpy(buffer + used, &request->nArgs, used);
  used *= 2;
  /* Insere todos os offsets necessarios */
  memcpy(buffer + used, offsets, sizeof(ssize_t) * request->nArgs);
  used += sizeof(ssize_t) * request->nArgs;

  /* Insere todos os argumentos */
  for(i = 0; i < request->nArgs; i++) {
    memcpy(buffer + used, request->argv[i], lens[i]);
    used += lens[i];
  }

  free(offsets);

  *length = byte_size ;

  return buffer;
}

Request deserialize_request(char* buffer, ssize_t length) {
  /* Deve haver pelo menos 2 * sizeof(int) bytes */
  unsigned int N, i = 0;
  Request request = NULL;
  char *tmp, *offsetdata = NULL;
  ssize_t arglen, *offsets, base;

  if(length >= 2 * sizeof(int)) {
    request = malloc(sizeof(struct request));

    // Estabelece o numero de pedido.
    request->ID = *(unsigned int*)buffer;

    // Estabelece o numero de argumentos.
    request->nArgs = *(unsigned int*)(buffer + sizeof(unsigned int));
    // Partindo disto, podemos construir os offsets;
    offsetdata = buffer + 2 * sizeof(unsigned int);
    offsets = (ssize_t*)malloc(sizeof(ssize_t) * request->nArgs);

    for(i = 0; i < request->nArgs; i++) {
      offsets[i] = *(ssize_t*)(offsetdata + sizeof(ssize_t) * i);
    }
    // Assim sendo, passa a ser possivel povoar os argumentos em si.
    request->argv = malloc(sizeof(char*) * (request->nArgs + 1));
    request->argv[request->nArgs] = NULL;

    N = request->nArgs;
    base = 2 * sizeof(unsigned int) + request->nArgs * sizeof(ssize_t);
    // Define os primeiros N - 1 argumentos;

    for(i = 1 ; i < N; i++) {
      arglen = offsets[i] - offsets[i - 1];
      tmp = malloc(sizeof(char) * (arglen + 1));

      memcpy(tmp, buffer + base + offsets[i - 1], arglen);

      tmp[arglen] = '\0';

      request->argv[i-1] = tmp;

    }
    // Define o ultimo argumento.
    if(request->nArgs > 0) {
      arglen = length - offsets[i-1] - base;
      tmp = malloc(sizeof(char) * (arglen + 1));
      memcpy(tmp, buffer + base + offsets[i-1], arglen);
      tmp[arglen] = '\0';
      request->argv[i-1] = tmp;
    }
    free(offsets);
  }

  return request;
}

Request request_pipe_timeout(char **argv, unsigned int length) {
  return request_geneic(SET_PIPE_TIMEOUT, argv, length);
}

Request request_exec_timeout(char **argv, unsigned int length) {
  return request_geneic(SET_EXEC_TIMEOUT, argv, length);
}

Request request_execute_task(char **argv, unsigned int length) {
  return request_geneic(EXECUTE_TASK, argv, length);
}

Request request_list_execution(char **argv, unsigned int length) {
  return request_geneic(LIST_IN_EXECUTION, argv, length);
}

Request request_kill_task(char **argv, unsigned int length) {
  return request_geneic(TERMINATE_TASK, argv, length);
}

Request request_list_history(char **argv, unsigned int length) {
  return request_geneic(LIST_HISTORY, argv, length);
}

Request request_spec_output(char **argv, unsigned int length) {
  return request_geneic(SPEC_OUTPUT, argv, length);
}
