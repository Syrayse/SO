#ifndef _REQUEST_SO_
#define _REQUEST_SO_

#include <sys/types.h>

typedef struct response {
    unsigned long ID;
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

#endif
