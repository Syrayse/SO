#include "Common.h"
#include "Request.h"
#include <time.h>
#include <stdio.h>

int pipe_reader, pipe_writer;

int main() {
  //srand(time(NULL));
  int i, N;
  pipe_writer = open(CL_TO_SR_PIPE, O_WRONLY);
  pipe_reader = open(SR_TO_CL_PIPE, O_RDONLY);

  ssize_t n, d;
  char buff[MAX_BUFFER_SIZE];

  if( !fork() ) {
    while( (n = read(pipe_reader, buff, MAX_BUFFER_SIZE)) > 0) {
      d = write(1, buff, n);

      if(d == -1)
        throw_error(2, "nao escreveu no stdout");
    }

    _exit(0);
  }

  while(1) {
    int N = rand() % (LIST_HISTORY + 1);
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
/*
  while( (n = read(0, buff, MAX_BUFFER_SIZE)) > 0 ) {
    d = write(fd1, buff, n);

    if(d == -1)
      throw_error(2, "nao escreveu no pipe de pipe certo");
  }*/

  return 0;
}
