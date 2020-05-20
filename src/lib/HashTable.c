#include "HashTable.h"
#include "List.h"

// ######################################################################
// Defines, e estruturas.

/**
 * Define numero maximo de entradas da tabela.
 */
#define MAX_SIZE 200


typedef struct hash_table {
  unsigned int num_elems;
  List table[MAX_SIZE];
} *HashTable;

// ######################################################################
// Funções Públicas.

HashTable hash_table_new(DestroyFunc free_func);
int hash_table_insert(HashTable table, unsigned int key, void* value);
int hash_table_contains(HashTable table, unsigned int key);
void* hash_table_find(HashTable table, unsigned int key);
unsigned int hash_table_size(HashTable table);
int hash_table_remove(HashTable table, unsigned int key);
void destroy_hash_table(HashTable table);

// ######################################################################
// Funções Privadas.

// ######################################################################
// Implementação.



#endif
