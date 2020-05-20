#include "Common.h"
#include "Request.h"
#include <stdio.h>

int main() {
  int i;
  int fd1 = open(CL_TO_SR_PIPE, O_RDONLY);
  int fd2 = open(SR_TO_CL_PIPE, O_WRONLY);
  ssize_t n;
  char buff[MAX_BUFFER_SIZE];

  n = read(fd1, buff, MAX_BUFFER_SIZE);
  Request r = deserialize_request(buff, n);

  printf("ID:%d\nnArgs:%d\n",r->ID, r->nArgs);
  for(i = 0; i < r->nArgs; i++)
    printf("\tword %d: %s\n", i, r->argv[i]);

  //write()


  /*while( (n = read(fd1, buff, MAX_BUFFER_SIZE)) > 0 ) {
    throw_error(fd2, "i got it");
    write(1, "GOT: ", 5);
    write(1, buff, n);
  }*/

  return 0;
}
