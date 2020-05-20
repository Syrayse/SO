
#include "Common.h"

int main() {
  int fd1 = open(CL_TO_SR_PIPE, O_RDONLY);
  int fd2 = open(SR_TO_CL_PIPE, O_WRONLY);
  ssize_t n;
  char buff[MAX_BUFFER_SIZE];

  while( (n = read(fd1, buff, MAX_BUFFER_SIZE)) > 0 ) {
    throw_error(fd2, "i got it");
    write(1, "GOT: ", 5);
    write(1, buff, n);
  }

  return 0;
}
