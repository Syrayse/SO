#include "Common.h"
#include <stdio.h>

ssize_t lastnl = -1;
ssize_t fst = -1;
ssize_t totalread = -1;
ssize_t buff_sz = -1;
char * buffer = NULL;

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

ssize_t readln(int fd, char* line, size_t size) {
	ssize_t len, tmp;
	char* nl;

	if(buffer == NULL) {
		buffer = malloc(sizeof(char)*MAX_BUFFER_SIZE);
		totalread = read(fd, buffer, MAX_BUFFER_SIZE);

    if(totalread == -1) {
      throw_error(2, "Error de leitura.");
      free(buffer);
      buffer = NULL;
      return -1;
    }

		buff_sz = MAX_BUFFER_SIZE;
	}

	for(;;) {
		len = totalread - (lastnl + 1);

		if((nl = memchr(buffer + lastnl + 1, '\n', len)) != NULL) {
			fst = lastnl + 1;
			lastnl = (ssize_t)(nl - buffer);
			break;
		}
		else {
			memcpy(buffer, buffer + lastnl + 1, len);
			lastnl = -1;
			totalread = len;
			buffer = realloc(buffer, 2*buff_sz);
			buff_sz *= 2;
			tmp = read(fd, buffer + totalread, buff_sz - totalread +1);
      if(tmp == -1) {
        throw_error(2, "Error de leitura.");
        free(buffer);
        buffer = NULL;
        return -1;
      }
			else
				totalread += tmp;
		}
	}

	if(lastnl != -1)
		memcpy(line, buffer + fst, lastnl - fst);

	return lastnl - fst;
}
