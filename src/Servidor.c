#include "Common.h"
#include "Request.h"
#include <stdio.h>

#define BASE_PIPE_TIMEOUT 10
#define BASE_EXEC_TIMEOUT 20

typedef void (*DispatchFunc)(char**);

unsigned int g_pipe_timeout = BASE_PIPE_TIMEOUT;
unsigned int g_exec_timeout = BASE_EXEC_TIMEOUT;
int pipe_reader, pipe_writer;

void process_pipe_timeout(char** argv) {
  g_pipe_timeout = atoi(argv[0]);
}

void process_exec_timeout(char** argv) {
  g_exec_timeout = atoi(argv[0]);
}

void process_exec_task(char** argv) {
  // Devera executar um novo processo
  // Novo processo serializa task e comunica o respectivo id.
  printf("exec task\n");
}

void process_list_execs(char** argv) {
  // Criar novo processo
  // Processo comunica tasks em aberto.
  printf("list execs\n");
}

void process_kill_task(char** argv) {
  // Envia um SIGKILL a uma task.
  // Assinala comando como COMMAND_TERMINATED
  printf("kill task\n");
}

void process_list_history(char** argv) {
  char* str = "";
  // Faz dump do processo de historico.
  ssize_t n = asprintf(&str, "the list is here sire\n");
  write(pipe_writer, str, n);
}

void setup_dispatcher(DispatchFunc *v) {
  v[SET_PIPE_TIMEOUT] = process_pipe_timeout;
  v[SET_EXEC_TIMEOUT] = process_exec_timeout;
  v[EXECUTE_TASK] = process_exec_task;
  v[LIST_IN_EXECUTION] = process_list_execs;
  v[TERMINATE_TASK] = process_kill_task;
  v[LIST_HISTORY] = process_list_history;
}


int main() {
  int i;
  pipe_reader = open(CL_TO_SR_PIPE, O_RDONLY);
  pipe_writer = open(SR_TO_CL_PIPE, O_WRONLY);
  ssize_t n;
  Request r;
  char buffer[MAX_BUFFER_SIZE];
  DispatchFunc req_dispatch[LIST_HISTORY + 1];
  setup_dispatcher(req_dispatch);

  while( (n = read(pipe_reader, buffer, MAX_BUFFER_SIZE)) > 0 ) {
    r = deserialize_request(buffer, n);

    printf("##################\n");
    printf("ID:%d\nnArgs:%d\n",r->ID, r->nArgs);
    for(i = 0; i < r->nArgs; i++)
      printf("\tword %d: %s\n", i, r->argv[i]);
    (*req_dispatch[r->ID])(r->argv);
    printf("##################\n");

    free(r);
  }

  /*
  n = read(fd1, buff, MAX_BUFFER_SIZE);



  printf("ID:%d\nnArgs:%d\n",r->ID, r->nArgs);
  for(i = 0; i < r->nArgs; i++)
    printf("\tword %d: %s\n", i, r->argv[i]);
  */
  //write()


  /*while( (n = read(fd1, buff, MAX_BUFFER_SIZE)) > 0 ) {
    throw_error(fd2, "i got it");
    write(1, "GOT: ", 5);
    write(1, buff, n);
  }*/

  return 0;
}
