#ifndef _REQUEST_SO_
#define _REQUEST_SO_

#include "Common.h"
#include <sys/types.h>

typedef struct response {
        int ID;
        unsigned long id_task;
        ssize_t length;
        char* buffer;
} * Response;

char* serialize_response(Response response, ssize_t* length);

Response deserialize_response(char* buffer, ssize_t length);

Response response_task_execute(unsigned long id_task);

Response response_task_killed(unsigned long id_task);

Response response_task_nen_exist(unsigned long id_task);

Response response_task_nen_exec(unsigned long id_task);

Response response_echo(char *buffer, ssize_t length);

ssize_t send_response(int fd, Response response);

#endif
