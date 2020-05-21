#include "Common.h"
#include "Request.h"
#include "LogManager.h"
#include <stdio.h>

#define BASE_PIPE_TIMEOUT 10
#define BASE_EXEC_TIMEOUT 20

typedef void (*DispatchFunc)(char**);

unsigned int g_pipe_timeout = BASE_PIPE_TIMEOUT;
unsigned int g_exec_timeout = BASE_EXEC_TIMEOUT;
int pipe_reader, pipe_writer;

void process_pipe_timeout(char** argv) {
  //g_pipe_timeout = atoi(argv[0]);
  printf("PIPE TIMEOUT\n");
}

void process_exec_timeout(char** argv) {
  //g_exec_timeout = atoi(argv[0]);
  printf("EXEC TIMEOUT\n");
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

void process_spec_output(char** argv) {
  // faz coisas
}

void setup_dispatcher(DispatchFunc *v) {
  v[SET_PIPE_TIMEOUT] = process_pipe_timeout;
  v[SET_EXEC_TIMEOUT] = process_exec_timeout;
  v[EXECUTE_TASK] = process_exec_task;
  v[LIST_IN_EXECUTION] = process_list_execs;
  v[TERMINATE_TASK] = process_kill_task;
  v[SPEC_OUTPUT] = process_spec_output;
  v[LIST_HISTORY] = process_list_history;
}


int main() {
  unsigned long x = init_log_file();

  get_buffer_info(2);
  printf("consegui %ld\n", x);

  redir_log_file(1);

  int i;
  unsigned long id = 0;
  pipe_reader = open(CL_TO_SR_PIPE, O_RDONLY);
  pipe_writer = open(SR_TO_CL_PIPE, O_WRONLY);
  ssize_t n;
  Request r;
  char buffer[MAX_BUFFER_SIZE];
  DispatchFunc req_dispatch[LIST_HISTORY + 1];
  setup_dispatcher(req_dispatch);

  while( (n = read(pipe_reader, buffer, MAX_BUFFER_SIZE)) > 0 && id < 5 ) {
    r = deserialize_request(buffer, n);

    printf("##################\n");
    printf("ID:%d\nnArgs:%d\n",r->ID, r->nArgs);
    for(i = 0; i < r->nArgs; i++)
      printf("\tword %d: %s\n", i, r->argv[i]);
    (*req_dispatch[r->ID])(r->argv);
    printf("##################\n");
    readapt_log_offset(id);

    free(r);
    id++;
  }

  return 0;
}
