/**
 * Ficheiro que contem defines transversais a todos os inputs.
 */
#ifndef _COMMON_SO_
#define _COMMON_SO_

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
  LIST_HISTORY
};

/**
 * Define o tipo de comportamento associado a um comando terminado.
 */
enum Command {
  COMMAND_SUCESS,
  COMMAND_TERMINATED,
  COMMAND_PIPE_TIMEOUT,
  COMMAND_EXEC_TIMEOUT,
};

void throw_error(int fd, char * msg);

#endif
