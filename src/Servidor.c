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
unsigned long id_pedido;

void redirect(int oldfd, int newfd) {
	if(oldfd != newfd) {
		dup2(oldfd, newfd);
		close(oldfd);
	}
}

void run(char * argv, int in, int out) {
  	redirect(in, STDIN_FILENO);
  	redirect(out, STDOUT_FILENO);

    // Para ja isto chega para testes, mas depois tenho de fazer
    // a minha propria funcao.
  	if( system(argv) == -1)
      throw_error(2, "Erro a executar commando.");

  	_exit(0);
}

void pipe_process(char *command[], int n) {

  	int i = 0, in = STDIN_FILENO;
  	for ( ; i < (n-1); ++i) {
	    int fd[2];
    	pid_t pid;

  		if( pipe(fd) == -1)
        throw_error(2, "Erro ao criar pipes.");

  		pid = fork();

    	if (pid == 0) {
      		close(fd[0]);
      		run(command[i], in, fd[1]);
    	}
    	else if (pid > 0) {
      		close(fd[1]);
      		close(in);
      		in = fd[0];
    	}
      else {
        throw_error(2, "Erro a executar comando.");
        return;
      }
  	}

  	run(command[i], in, STDOUT_FILENO);
}

void process_line(char *str) {
	int i, N = 0;
	char **command = malloc(sizeof(char*)*10);
	char *token = strtok(str, "|");

	while(token != NULL) {
		command[N++] = strdup(token);
		token = strtok(NULL,"|");
	}

	pipe_process(command, N);

	for(i = 0; i < N; i++) {
		free(command[i]);
	}

	free(command);

}

void update_indexes(int signum) {
	readapt_log_offset(id_pedido);
	id_pedido++;
}

void process_pipe_timeout(char** argv) {
  //g_pipe_timeout = atoi(argv[0]);
  printf("PIPE TIMEOUT\n");
}

void process_exec_timeout(char** argv) {
  //g_exec_timeout = atoi(argv[0]);
  printf("EXEC TIMEOUT\n");
}

void process_exec_task(char** argv) {
  pid_t pid;
  ssize_t n;
  char * str = "";

  // Devera executar um novo processo
  // Novo processo serializa task e comunica o respectivo id.
  if(!fork()) {
    redir_log_file(1);

    pid = fork();

    if(!pid) {
      process_line(argv[0]);
      _exit(0);
    }
    else {
      wait(NULL);

			kill(getppid(), SIGUSR1);

      n = asprintf(&str, "nova tarefa #%ld.\n", id_pedido);

      if(write(pipe_writer, str, n) == -1)
        throw_error(2, "Erro ao submeter info no pipe.");
    }

    _exit(0);
  }
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
	char * str = "";
	unsigned long id = strtoul(argv[0],&str,0);
	ssize_t n;

  // Devera executar um novo processo
	// Potencialmente o processo original pode ser enorme.
  if(!fork()) {
		n = asprintf(&str, "> A enviar output da tarefa %ld.\n", id);

		if( write(1, str, n) == -1)
			throw_error(2, "Nao conseguiu escrever no output.");

    if( get_buffer_info(pipe_writer, id) == -1)
			throw_error(2, "Impossivel aceder a informacao dos logs.");
    _exit(0);
  }
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
	// Sinal que e enviado para o processo principal
	// assim que processos filhos terminam emitem este
	// sinal ao processo pai, de forma a notificar que
	// pode atualizar o log_manager, por a informacao de output
	// do processo filho ja foi inserida.
	signal(SIGUSR1, update_indexes);

  id_pedido = init_log_file();

  int i;
  pipe_reader = open(CL_TO_SR_PIPE, O_RDONLY);
  pipe_writer = open(SR_TO_CL_PIPE, O_WRONLY);
  ssize_t n;
  Request r;
  char buffer[MAX_BUFFER_SIZE];
  DispatchFunc req_dispatch[LIST_HISTORY + 1];
  setup_dispatcher(req_dispatch);

	while(1) {
	  if( (n = read(pipe_reader, buffer, MAX_BUFFER_SIZE)) > 0 ) {
	    r = deserialize_request(buffer, n);

	    printf("##################\n");
	    printf("ID:%d\nnArgs:%d\n",r->ID, r->nArgs);
	    for(i = 0; i < r->nArgs; i++)
	      printf("\tword %d: %s\n", i, r->argv[i]);
	    (*req_dispatch[r->ID])(r->argv);
	    printf("##################\n");

	    free(r);
	  }
	}
  return 0;
}
