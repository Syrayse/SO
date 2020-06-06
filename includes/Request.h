#ifndef _REQUEST_SO_
#define _REQUEST_SO_

#include <sys/types.h>

typedef struct request {
    unsigned int ID;
    unsigned int nArgs;
    char** argv;
} * Request;

char* serialize_request(Request request, ssize_t* length);

Request deserialize_request(char* buffer, ssize_t length);

Request request_pipe_timeout(char** argv, unsigned int length);

Request request_exec_timeout(char** argv, unsigned int length);

Request request_execute_task(char** argv, unsigned int length);

Request request_list_execution(char** argv, unsigned int length);

Request request_kill_task(char** argv, unsigned int length);

Request request_list_history(char** argv, unsigned int length);

Request request_spec_output(char** argv, unsigned int length);

#endif
