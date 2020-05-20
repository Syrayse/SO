#include "Common.h"
#include "Request.h"

int main() {
  int fd1 = open(CL_TO_SR_PIPE, O_WRONLY);
  int fd2 = open(SR_TO_CL_PIPE, O_RDONLY);
  ssize_t n, d;
  char buff[MAX_BUFFER_SIZE];

  Request r = malloc(sizeof(struct request));
  r->ID = 123;
  r->nArgs = 2;
  r->argv = malloc(sizeof(char*)*2);
  r->argv[0] = strdup("lol1");
  r->argv[1] = strdup("lol2");

  char * buffer = serialize_request(r, &n);

  write(fd1, buffer, n);

  /*
  if( !fork() ) {
    while( (n = read(fd2, buff, MAX_BUFFER_SIZE)) > 0) {
      d = write(1, buff, n);

      if(d == -1)
        throw_error(2, "nao escreveu no stdout");
    }

    _exit(0);
  }

  while( (n = read(0, buff, MAX_BUFFER_SIZE)) > 0 ) {
    d = write(fd1, buff, n);

    if(d == -1)
      throw_error(2, "nao escreveu no pipe de pipe certo");
  }*/

  return 0;
}
