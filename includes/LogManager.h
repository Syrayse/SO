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
unsigned long init_log_file();

ssize_t get_offset_size(unsigned long ID, ssize_t *size);

int redir_log_file(int fd);

ssize_t get_buffer_info(int fd, unsigned long ID);

void readapt_log_offset(unsigned long ID);

#endif
