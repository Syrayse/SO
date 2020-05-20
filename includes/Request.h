#ifndef _REQUEST_SO_
#define _REQUEST_SO_

typedef struct request  {
  unsigned int ID;
  unsigned int nArgs;
  char **argv;
} *Request;

char* serialize_request(Request request, ssize_t *length);

Request deserialize_request(char* buffer, ssize_t length);

#endif
