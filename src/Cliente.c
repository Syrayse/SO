
#include "Common.h"

int main() {
  int fd1 = open(CL_TO_SR_PIPE, O_WRONLY);
  int fd2 = open(SR_TO_CL_PIPE, O_RDONLY);
  ssize_t n, d;
  char buff[MAX_BUFFER_SIZE];

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
  }

  return 0;
}
