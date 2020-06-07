#include "Common.h"
#include "HashTable.h"
#include "LogManager.h"
#include "Request.h"
#include <stdio.h>

#define BASE_PIPE_TIMEOUT 10
#define BASE_EXEC_TIMEOUT 20

typedef void (*DispatchFunc)(char**);

typedef struct taskinfo {
        pid_t care_taker;
        char* command;
} * TaskInfo;

unsigned int g_pipe_timeout = BASE_PIPE_TIMEOUT;
unsigned int g_exec_timeout = BASE_EXEC_TIMEOUT;
int pipe_reader, pipe_writer, terminate_pipe[2];
unsigned long id_pedido;
HashTable table;

void process_termination(int signum)
{
        unsigned long id;

        close(terminate_pipe[1]);

        if (read(terminate_pipe[0], &id,
                 sizeof(unsigned long))
            == -1) {
                throw_error(2, "error ao ler do pipe.");
                return;
        }

        close(terminate_pipe[0]);
        hash_table_remove(table, id);
}

void foreach_task(unsigned long key, void* value)
{
        char* buff = "";
        TaskInfo ti = (TaskInfo)value;
        ssize_t n = asprintf(&buff, "#%ld: %s\n", key, ti->command);

        if (n && write(pipe_writer, buff, n) == -1)
                throw_error(2, "Erro na escrita com foreach.");
}

void redirect(int oldfd, int newfd)
{
        if (oldfd != newfd) {
                dup2(oldfd, newfd);
                close(oldfd);
        }
}

int run(char* argv, int in, int out)
{
        int r = 0, size;
        redirect(in, STDIN_FILENO);
        redirect(out, STDOUT_FILENO);

        // faz o tokenizing da string indicada.
        char **tokens = specialized_tok(argv, '\"', &size);

        if(size > 0) {
                r = execvp(tokens[0], tokens);
        }

        _exit(1);

        return r;
}

int pipe_process(char* command[], int n)
{
        int nd = 0;
        int i = 0, in = STDIN_FILENO;
        for (; i < (n - 1); ++i) {
                int fd[2];
                pid_t pid;

                if (pipe(fd) == -1) {
                        throw_error(2, "Erro ao criar pipes.");
                        return -1;
                }

                pid = fork();

                if (pid == 0) {
                        close(fd[0]);
                        nd = run(command[i], in, fd[1]);

                        if (nd == -1)
                                return -1;
                } else if (pid > 0) {
                        close(fd[1]);
                        close(in);
                        in = fd[0];
                } else {
                        throw_error(2, "Erro a executar comando.");
                        return -1;
                }
        }

        nd = run(command[i], in, STDOUT_FILENO);
        return nd;
}

int process_line(char* str)
{
        int t, i, N = 0;
        char** command = malloc(sizeof(char*) * 10);
        char* token = strtok(str, "|");

        while (token != NULL) {
                command[N++] = strdup(token);
                token = strtok(NULL, "|");
        }

        t = pipe_process(command, N);

        for (i = 0; i < N; i++) {
                free(command[i]);
        }

        free(command);
        return t;
}

void update_indexes(int signum)
{
        readapt_log_offset(id_pedido);
        id_pedido++;
}

void process_pipe_timeout(char** argv)
{
        g_pipe_timeout = atoi(argv[0]);
}

void process_exec_timeout(char** argv)
{
        g_exec_timeout = atoi(argv[0]);
}

void process_exec_task(char** argv)
{
        pid_t m, pid;
        int status;
        if (pipe(terminate_pipe) == -1)
                throw_error(2, "error a criar pipe anonimo.");
        enum Command c = COMMAND_SUCESS;

        // Devera executar um novo processo
        // Novo processo serializa task e comunica o respectivo id.
        m = fork();

        if (!m) {
                setpgrp();
                redir_log_file(1);

                pid = fork();


                if (!pid) {
                        status = process_line(argv[0]);
                        _exit(status == -1 ? 1 : 0);
                } else {
                        Response resp = response_task_execute(id_pedido);

                        send_response(pipe_writer, resp);

                        free(resp);

                        waitpid(pid, &status, WUNTRACED);

                        kill(getppid(), SIGUSR1);

                        if (WEXITSTATUS(status) != 0) {
                                c = COMMAND_ERROR;
                        }

                        append_task_info(id_pedido, argv[0], c);

                        kill(getppid(), SIGUSR2);

                        close(terminate_pipe[0]);

                        if (write(terminate_pipe[1], &id_pedido,
                                  sizeof(unsigned long))
                            == -1)
                                throw_error(2, "error na escrita do pipe.");

                        close(terminate_pipe[1]);
                }

                _exit(0);
        }

        TaskInfo ti = malloc(sizeof(struct taskinfo));
        ti->care_taker = m;
        ti->command = strdup(argv[0]);
        hash_table_insert(table, id_pedido, ti);
}

void process_list_execs(char** argv)
{
        char c = '\0';

        hash_table_foreach(table, foreach_task);

        if (write(pipe_writer, &c, 1) == -1)
                throw_error(2, "Erro inesperado na escrita.");
}

void process_kill_task(char** argv)
{
        char* str = "";
        unsigned long id = strtoul(argv[0], &str, 0);
        TaskInfo info = hash_table_find(table, id);

        if (info == NULL) {
                // Aquele id nao esta em execucao,
                // envia msg a notificar
                send_response(pipe_writer, response_task_nen_exec(id));
        } else {
                // Aquele pedido está contido na hash
                // table, deve por isso ser morto e removido.
                kill(-info->care_taker, SIGKILL);
                hash_table_remove(table, id);

                // Para alem disso, e tambem preciso
                // inserir nos logs a causa da sua morte.
                append_task_info(id, info->command,
                                 COMMAND_TERMINATED);

                send_response(pipe_writer, response_task_killed(id));
        }
}

void process_list_history(char** argv)
{
        dump_task_history(pipe_writer);
}

void process_spec_output(char** argv)
{
        char* str = "";
        unsigned long id = strtoul(argv[0], &str, 0);
        ssize_t n;

        // Devera executar um novo processo
        // Potencialmente o processo original pode ser enorme.
        if (!fork()) {
                n = asprintf(&str, "> A enviar output da tarefa %ld.\n", id);

                if (write(1, str, n) == -1)
                        throw_error(2, "Nao conseguiu escrever no output.");

                if ((n = get_buffer_info(pipe_writer, id)) == -1)
                        throw_error(2, "Impossivel aceder a informacao dos logs.");

                _exit(0);
        }
}

void setup_dispatcher(DispatchFunc* v)
{
        v[SET_PIPE_TIMEOUT] = process_pipe_timeout;
        v[SET_EXEC_TIMEOUT] = process_exec_timeout;
        v[EXECUTE_TASK] = process_exec_task;
        v[LIST_IN_EXECUTION] = process_list_execs;
        v[TERMINATE_TASK] = process_kill_task;
        v[SPEC_OUTPUT] = process_spec_output;
        v[LIST_HISTORY] = process_list_history;
}

int main()
{
        table = hash_table_new(free);
        // Sinal que e enviado para o processo principal
        // assim que processos filhos terminam emitem este
        // sinal ao processo pai, de forma a notificar que
        // pode atualizar o log_manager, por a informacao de output
        // do processo filho ja foi inserida.
        signal(SIGUSR1, update_indexes);
        signal(SIGUSR2, process_termination);

        id_pedido = init_log_file();

        pipe_reader = open(CL_TO_SR_PIPE, O_RDONLY);
        pipe_writer = open(SR_TO_CL_PIPE, O_WRONLY);
        ssize_t n;
        Request r;
        char buffer[MAX_BUFFER_SIZE];
        DispatchFunc req_dispatch[LIST_HISTORY + 1];
        setup_dispatcher(req_dispatch);

        while ((n = read(pipe_reader, buffer, MAX_BUFFER_SIZE)) >= 0) {
                if(n) {
                        r = deserialize_request(buffer, n);

                        (*req_dispatch[r->ID])(r->argv);

                        free(r);
                }
        }

        destroy_hash_table(table);

        return 0;
}
