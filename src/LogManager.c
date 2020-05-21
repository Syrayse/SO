#include "LogManager.h"
#include <stdio.h>

int log_file, log_idx_file;
off_t curr_offset, log_offset;
unsigned long num_elems;

unsigned long entry_size() {
  return (sizeof(unsigned long) + sizeof(off_t) + sizeof(ssize_t));
}

unsigned long init_log_file() {
  log_file = open(LOG_FILE, O_CREAT | O_RDWR, 0666);
  log_idx_file = open(LOG_IDX_FILE, O_CREAT | O_RDWR, 0666);
  log_offset = lseek(log_file, 0, SEEK_END);
  curr_offset = lseek(log_idx_file, 0, SEEK_END);
  num_elems = curr_offset / entry_size();
  return num_elems;
}

ssize_t get_offset_size(unsigned long ID, ssize_t *size) {
  ssize_t w, offset = -1;
  unsigned long N = entry_size();
  off_t jump, dest_off = ID * N;
  char * buffer = malloc(sizeof(char) * N);

  // Se o ID está acima do EOF.
  if(dest_off > num_elems * N)
    return offset;

  // Caso contrario, esse ID já foi indicado.
  // e preciso calcular o jump preciso.
  // se co for 70 e do 60, jump = -10
  // se co for 60 e do 70, jump = 10.
  jump = dest_off - curr_offset;

  // Saltar ate essa posicao
  curr_offset = lseek(log_idx_file, jump, SEEK_CUR);

  // Le aquela entrada
  w = read(log_idx_file, buffer, N);

  if(w == -1)
    throw_error(2, "Nao conseguiu ler log file");

  offset = *(ssize_t*)(buffer + sizeof(unsigned long));
  *size = *(ssize_t*)(buffer + N - sizeof(ssize_t));

  return offset;
}

int redir_log_file(int fd) {
  return dup2(log_file, 1);
}

void readapt_log_offset(unsigned long ID) {
  unsigned long i, N = entry_size();
  off_t newoff = lseek(log_file, 0, SEEK_END);
  off_t size = newoff - log_offset;
  ssize_t w, jump, used = 0;
  char buffer[MAX_BUFFER_SIZE],buffer2[MAX_BUFFER_SIZE], c = 0;
  // copy ID
  memcpy(buffer, &ID, sizeof(unsigned long));
  // copy offsets
  memcpy(buffer + sizeof(unsigned long), &log_offset, sizeof(off_t));
  // copy length
  memcpy(buffer + sizeof(unsigned long) + sizeof(off_t), &size, sizeof(off_t));

  if(ID >= num_elems) {
    // adicionar padding ate a posicao necessaria.
    curr_offset = lseek(log_idx_file, 0, SEEK_END);

    for(i = num_elems; i < ID; i++) {
      memcpy(buffer2 + i - num_elems, &c, 1);
      used++;
    }

    w = write(log_idx_file, buffer2, used);

    if(w == -1) {
      throw_error(2, "Error inesperado na escrita.");
      return;
    }

    curr_offset += w;

    w = write(log_idx_file, buffer, N);

    if(w == -1) {
      throw_error(2, "Error inesperado na escrita.");
      return;
    }

    num_elems = ID + 1;
  }
  else {
    // Podemos ir a posicao e simplesmente inserir la.
    jump = ID * N - curr_offset;
    curr_offset = lseek(log_idx_file, jump, SEEK_CUR);

    w = write(log_idx_file, buffer, N);

    if(w == -1) {
      throw_error(2, "Error inesperado na escrita.");
      return;
    }

  }

  curr_offset += w;

  // De qualquer das formas, volta ao fim.
  log_offset = lseek(log_file, 0, SEEK_END);
}

ssize_t get_buffer_info(unsigned long ID) {
  char strbuff[MAX_BUFFER_SIZE];
  ssize_t w, size, tmp, procd = 0;
  ssize_t offset = get_offset_size(ID, &size);

  if(offset == -1) {
    sprintf(strbuff, "Nao existe algum ID %ld na base.", ID);
    throw_error(2, strbuff);
    return -1;
  }

  // Saltar para a posicao destino
  lseek(log_file, offset - log_offset, SEEK_CUR);

  // Escreve no stdout cada MAX_BUFFER_SIZE que encontrar
  // até ao fim.
  while(procd < size) {
    tmp = read(log_file, strbuff, MAX_BUFFER_SIZE);

    if(tmp == -1) {
      throw_error(2, "Nao conseguiu ler ficheiro de logs.");
      return -1;
    }

    w = write(1, strbuff, tmp);

    if(w == -1) {
      throw_error(2, "Error inesperado na escrita.");
      lseek(log_file, 0, SEEK_END);
      return size;
    }

    procd += tmp;
  }

  // Saltar novamente para o fim
  lseek(log_file, 0, SEEK_END);

  return size;
}
