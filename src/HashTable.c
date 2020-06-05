#include "HashTable.h"
#include <stdlib.h>

/**
 * Define numero maximo de entradas da tabela.
 */
#define MAX_SIZE 200

typedef void (*DestroyFunc)(void*);

typedef struct list {
  unsigned long key;
  void *entry;
  struct list *next;
}*List;

typedef struct hash_table {
  DestroyFunc funf;
  unsigned long num_elems;
  List table[MAX_SIZE];
} *HashTable;

List new_list_node(unsigned long key, void *value) {
  List r = malloc(sizeof(struct list));

  r->key = key;
  r->entry = value;
  r->next = NULL;

  return r;
}

HashTable hash_table_new(DestroyFunc free_func) {
  int i;
  HashTable table = malloc(sizeof(struct hash_table));

  for(i = 0; i < MAX_SIZE; i++) {
    table->table[i] = NULL;
  }

  table->num_elems = 0;
  table->funf = free_func;

  return table;
}

int hash_table_insert(HashTable table, unsigned long key, void* value) {
  int r = 0;
  unsigned long hash = key % MAX_SIZE;
  List *el, node = new_list_node(key, value);

  for(el = &table->table[hash] ; *el && (*el)->key != key ; el = &(*el)->next)
    ;

  if(!*el) {
    *el = node;
    r = 1;
  }

  return r;
}

int hash_table_contains(HashTable table, unsigned long key) {
  List it;
  unsigned long hash = key % MAX_SIZE;

  for(it = table->table[hash] ; it && it->key != key ; it = it->next)
    ;

  return it != NULL;
}

void* hash_table_find(HashTable table, unsigned long key) {
  List it;
  void *res = NULL;
  unsigned long hash = key % MAX_SIZE;

  for(it = table->table[hash] ; it && it->key != key ; it = it->next)
    ;

  if(it)
    res = it->entry;

  return res;
}

unsigned long hash_table_size(HashTable table) {
  return table->num_elems;
}

int hash_table_remove(HashTable table, unsigned long key) {
  int r = 0;
  unsigned long hash = key % MAX_SIZE;
  List *el, tmp;

  for(el = &table->table[hash] ; *el && (*el)->key != key ; el = &(*el)->next)
    ;

  if(*el) {
    tmp = *el;
    *el = (*el)->next;

    if(table->funf)
      (*table->funf)(tmp);

    r = 1;
  }

  return r;
}

void hash_table_foreach(HashTable table, ForeachFunc feach) {
  List it;
  int i;
  if(feach) {
    for(i = 0; i < MAX_SIZE; i++) {

      for(it = table->table[i]; it; it = it->next)
        (*feach)(it->key, it->entry);

    }
  }
}

void destroy_hash_table(HashTable table) {
  int i;
  List t,tmp;

  for(i = 0; i < MAX_SIZE; i++) {
    for(t = table->table[i] ; t; ) {
      tmp = t;
      t = t->next;

      if(table->funf) {
        (*table->funf)(tmp->entry);
      }

      free(tmp);
    }

    table->table[i] = NULL;
  }

}
