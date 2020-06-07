#include "Response.h"
#include "Common.h"

Response response_generic(unsigned long ID, unsigned long id_task,
                        ssize_t length, char * buffer) {
      Response r = malloc(sizeof(struct response));

      r->ID = ID;
      r->id_task = id_task;
      r->length = length;
      r->buffer = buffer;

      return r;
}

char* serialize_response(Response response, ssize_t* length) {
      char *buffer = NULL;
      unsigned long sz = 0;

      if(response->ID == ECHO) {
            sz = sizeof(unsigned long) + sizeof(ssize_t) + response->length;
            buffer = malloc(sizeof(char) * sz);

            // insere ID
            memcpy(buffer, &(response->ID), sizeof(unsigned long));

            // Insere length
            memcpy(buffer + sizeof(ssize_t), &(response->id_task), sizeof(ssize_t));

            // Insere buffer
            memcpy(buffer + sizeof(ssize_t) + sizeof(unsigned long), response->buffer, response->length);
      } else {
            sz = sizeof(unsigned long)*2;
            buffer = malloc(sizeof(char) * sz);

            // Insere ID
            memcpy(buffer, &(response->ID), sizeof(unsigned long));

            // Insere id_task
            memcpy(buffer + sizeof(unsigned long), &(response->id_task), sizeof(unsigned long));
      }

      *length = sz;

      return buffer;
}

Response deserialize_response(char* buffer, ssize_t length) {
      Response r = malloc(sizeof(struct response));
      unsigned long ID = *(unsigned long*)buffer;

      if(ID == ECHO) {
            r->id_task = 0;
            r->length = *(ssize_t*)(buffer + sizeof(unsigned long));
            r->buffer = buffer + sizeof(unsigned long) + sizeof(ssize_t);
      } else {
            r->id_task = *(unsigned long*)(buffer + sizeof(unsigned long));
            r->length = -1;
            r->buffer = NULL;
      }

      return r;
}

Response response_task_execute(unsigned long id_task) {
      return response_generic(TASK_EXECUTE, id_task, -1, NULL);
}

Response response_task_killed(unsigned long id_task) {
      return response_generic(TASK_KILLED, id_task, -1, NULL);
}

Response response_task_nen_exist(unsigned long id_task) {
      return response_generic(TASK_NEN_EXIST, id_task, -1, NULL);
}

Response response_task_nen_exec(unsigned long id_task) {
      return response_generic(TASK_NEN_EXEC, id_task, -1, NULL);
}

Response response_echo(char *buffer, ssize_t length) {
      return response_generic(ECHO, 0, length, buffer);
}
