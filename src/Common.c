#include "Common.h"

#include <ctype.h>
#include <stdio.h>

enum STATES { WHITESPACE,
    WORD,
    STRING };

void throw_error(int fd, char* msg)
{
    char* str = "";
    ssize_t w = asprintf(&str, "ERROR: %s\n", msg);

    // Erro a alocar espaço da string.
    if (w == -1)
        return;

    ssize_t n = write(fd, str, strlen(str));

    // Erro a escrever o erro
    if (n == -1)
        ;

    free(str);
}

ssize_t send_response(int fd, Response response) {
      ssize_t size;
      char * buffer = serialize_response(response, &size);

      if( write(fd, buffer, size) == -1 ) {
            throw_error(2, "erro na escrita.");
            size = -1;
      }

      free(buffer);

      return size;
}

ssize_t readln(int fd, char* line, size_t size)
{
      char c = '\n';
    ssize_t i, t_read = 0, ind;

    // Le tudo, de seguida um a um analisa.
    t_read = read(fd, line, size);

    if (t_read == -1) {
        throw_error(2, "erro na escrita.");
        return -1;
 } else if (!t_read) {
       if( write(fd, &c, 1) == -1)
            throw_error(2, "erro na escrita.");
          return -1;
    }

    for (i = 0; i < t_read && line[i] != c; i++)
        ;

    ind = (i == t_read) ? 0 : i;

    // COlocar o terminador
    line[ind] = '\0';

    return ind;
}

char** specialized_tok(char* line, char delim, int* final_size)
{
    // Esta funcao efetua um tokenizing de acordo espacos
    // em branco, e ` como separador de string.

    // valor a ser retornado, contem os tokens todos,
    // deve ser null-terminated.
    char tmp, **res = malloc(sizeof(char*) * 100);

    // comeca com o estado vazio.
    enum STATES state = WHITESPACE;

    // tamanho total da string.
    int i, start, size = 0;

    for (i = 0; line[i]; i++) {

        tmp = line[i];

        switch (state) {
        // if in whitespace.
        case WHITESPACE:
            // if whitespace, ignore
            if (!isspace(tmp)) {
                // if first letter is '`', changes to STRING.
                if (tmp == delim) {
                    start = i + 1;
                    state = STRING;
                } else {
                    start = i;
                    state = WORD;
                }
            }
            break;
        // if reading a word.
        case WORD:
            // if  whitespace, and add word to buffer.
            if (isspace(tmp)) {
                line[i] = '\0';
                res[size++] = strdup(line + start);
                line[i] = tmp;

                // state is now WHITESPACE
                state = WHITESPACE;
            }
            // otherwise, do nothing.
            break;
        // if reading a string.
        case STRING:
            // if whitespace ignore, if reached end `, add file.
            if (tmp == delim) {
                line[i] = '\0';
                res[size++] = strdup(line + start);
                line[i] = tmp;

                // state is now whitespace
                state = WHITESPACE;
            }
            break;
        }
    }

    // if finished whilst some state other than whitespace, add it.
    if (state != WHITESPACE) {
        res[size++] = strdup(line + start);
    }

    // make it null terminated.
    res[size] = NULL;

    *final_size = size;

    return res;
}
