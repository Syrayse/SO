#ifndef _LOG_MAN_SO_
#define _LOG_MAN_SO_

#include "Common.h"

#define LOG_FILE      "log"

#define LOG_IDX_FILE  "log.idx"

/**
 * Inicializa os ficheiros log_file e LOG_IDX_FILE.
 *
 * @return O ID que atualmente está a ser processado.
 */
unsigned int init_log_file();

ssize_t get_offset_size(unsigned int ID, ssize_t *size);

int redir_log_file(int fd);

char * get_buffer_info(unsigned int ID);

#endif
