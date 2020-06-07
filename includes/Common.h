/**
 * Ficheiro que contem defines transversais a todos os inputs.
 */
#ifndef _COMMON_SO_
#define _COMMON_SO_

#define _GNU_SOURCE

#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Response.h"

/**
 * Define o tamanho de buffer default do sistema.
 */
#define MAX_BUFFER_SIZE 10240

/**
 * Define o pipe com nome utilizado nas comunicações do cliente para o
 * server.
 */
#define CL_TO_SR_PIPE "cl2srpipe"

/**
 * Define o pipe com nome utilizado nas comunicações do server para o
 * cliente.
 */
#define SR_TO_CL_PIPE "sr2clpipe"

/**
 * Define todos os códigos para processamento.
 */
enum Request {
    SET_PIPE_TIMEOUT = 0,
    SET_EXEC_TIMEOUT,
    EXECUTE_TASK,
    LIST_IN_EXECUTION,
    TERMINATE_TASK,
    SPEC_OUTPUT,
    LIST_HISTORY
};

enum Response {
      TASK_EXECUTE = 0,
      TASK_KILLED,
      TASK_NEN_EXIST,
      TASK_NEN_EXEC,
      ECHO
};

/**
 * Define o tipo de comportamento associado a um comando terminado.
 */
enum Command {
    COMMAND_SUCESS,
    COMMAND_ERROR,
    COMMAND_TERMINATED,
    COMMAND_PIPE_TIMEOUT,
    COMMAND_EXEC_TIMEOUT,
};

void throw_error(int fd, char* msg);
ssize_t readln(int fd, char* line, size_t size);
char** specialized_tok(char* line, char delim, int* final_size);

#endif
