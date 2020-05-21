#include "Common.h"
#include "Request.h"
#include <stdio.h>
#include "LogManager.h"

int pipe_reader, pipe_writer, read_server = 1;

void dont_read_server(int signum) {
  read_server = 0;
}

void deliver_request(Request request) {
  ssize_t d, size;
  char * buffer = NULL;

  buffer = serialize_request(request, &size);

  if(buffer) {
    d = write(pipe_writer, buffer, size);

    if(d == -1)
      throw_error(2, "Nao foi possivel escrever no pipe.");

    free(buffer);
  }
  else
    throw_error(2, "Impossivel serializar pedido.");

  free(request);
}

void parse_shell() {
  int i, N;
  ssize_t n;
  while(1) {
    N = rand() % 5;
    Request r = malloc(sizeof(struct request));
    r->ID = N;
    r->nArgs = N;
    r->argv = malloc(sizeof(char*)*N);
    for(i = 0; i < N; i++) {
      r->argv[i] = strdup("100");
    }

    char * buffer = serialize_request(r, &n);

    write(pipe_writer, buffer, n);

    free(buffer);
    sleep(1);
  }
}

void parse_shell_inputs(int argc, char *argv[]) {
  int allok = 0;
  ssize_t n, d;
  Request request = NULL;
  char * buff = "";

  if( !strcmp(argv[0], "tempo-inactividade") && argc > 1) {
    request = request_pipe_timeout(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "tempo-execucao") && argc > 1 ) {
    request = request_exec_timeout(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "executar") && argc > 1 ) {
    request = request_execute_task(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "terminate") && argc > 1 ) {
    request = request_kill_task(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "output") && argc > 1 ) {
    request = request_spec_output(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "listar") ) {
    request = request_list_execution(argv, 0);
    allok = 1;
  }
  else if( !strcmp(argv[0], "historico") ) {
    request = request_list_history(argv, 0);
    allok = 1;
  }
  else if( !strcmp(argv[0], "ajuda") ) {
    n = asprintf(&buff, "tempo-inactividade segs\ntempo-execucao segs\n executar \"p1 | p2 ... | p3\"\nlistar\nterminar [id tarefa]\nhistorico\najuda\noutput [id tarefa]\n");
    d = write(1, buff, n);

    if(d == -1)
      throw_error(2, "Impossivel escrever no output.");

    allok = 1;
  }

  if(!allok) {
    throw_error(2, "Numero de argumentos invalido.");
  }
  else if( request ) {
    deliver_request(request);
  }
}

void parse_arguments(int argc, char *argv[]) {
  int allok = 0;
  ssize_t n, d;
  Request request = NULL;
  char * buff = "";

  if( !strcmp(argv[0], "-i") && argc > 1) {
    request = request_pipe_timeout(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-m") && argc > 1 ) {
    request = request_exec_timeout(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-e") && argc > 1 ) {
    request = request_execute_task(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-t") && argc > 1 ) {
    request = request_kill_task(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-o") && argc > 1 ) {
    request = request_spec_output(argv + 1, 1);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-l") ) {
    request = request_list_execution(argv, 0);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-r") ) {
    request = request_list_history(argv, 0);
    allok = 1;
  }
  else if( !strcmp(argv[0], "-h") ) {
    n = asprintf(&buff, "-i segs\n-m segs\n-e \"p1 | p2 ... | pn\"\n-l\n-t [id tarefa]\n-r\n-h\n-o [id tarefa]\n");
    d = write(1, buff, n);

    if(d == -1)
      throw_error(2, "Impossivel escrever no output.");

    allok = 1;
  }

  if(!allok) {
    throw_error(2, "Numero de argumentos invalido.");
  }
  else if( request ) {
    deliver_request(request);
  }
}

int main(int argc, char *argv[]) {
  pid_t son;
  pipe_writer = open(CL_TO_SR_PIPE, O_WRONLY);
  pipe_reader = open(SR_TO_CL_PIPE, O_RDONLY);
  signal(SIGUSR1, dont_read_server);

  ssize_t n, d;
  unsigned long used = 0;
  char buff[MAX_BUFFER_SIZE];

  /* Um dos forks fica sempre a imprimir o que o provem do servidor. */
  son = fork();
  if( !son ) {
    while( read_server &&
        (n = read(pipe_reader, buff, MAX_BUFFER_SIZE)) > 0) {
      d = write(1, buff, n);

      if(d == -1)
        throw_error(2, "Nao escreveu no stdout.");
    }

    _exit(0);
  }

  if(argc == 1) {
    // Entrar em modo shell
    parse_shell();
  }
  else {
    // Entrar em modo por argumentos.
    parse_arguments(argc - 1, argv + 1);
  }

  // É necessario esperar que o filho, que processa msgs do server, termine.
  // Notifica a thread filha que deverá terminar assim que receber a resposta
  // seguinte do servidor. Devemos fazer isto para evitar terminar a execucao do
  // argus sem antes de obter uma resposta do servidor.

  d = kill(son, SIGUSR1);

  if(d == -1)
    throw_error(2, "Impossivel enviar sinal.");

  waitpid(son, NULL, WUNTRACED);

  return 0;
}
