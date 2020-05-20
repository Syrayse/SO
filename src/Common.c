#define _GNU_SOURCE
#include "Common.h"
#include <stdio.h>

void throw_error(int fd, char * msg) {
  char * str = "" ;
  ssize_t w = asprintf(&str, "ERROR: %s\n", msg);

  // Erro a alocar espaço da string.
  if(w == -1)
    return;

  ssize_t n = write(fd, str, strlen(str));

  // Erro a escrever o erro
  if(n == -1);

  free(str);
}
