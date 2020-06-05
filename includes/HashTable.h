#ifndef _HASH_TABLE_SO_
#define _HASH_TABLE_SO_

typedef void (*DestroyFunc)(void*);

typedef void (*ForeachFunc)(unsigned long, void*);

typedef struct hash_table *HashTable;

HashTable hash_table_new(DestroyFunc free_func);

int hash_table_insert(HashTable table, unsigned long key, void* value);

int hash_table_contains(HashTable table, unsigned long key);

void* hash_table_find(HashTable table, unsigned long key);

unsigned long hash_table_size(HashTable table);

int hash_table_remove(HashTable table, unsigned long key);

void hash_table_foreach(HashTable table, ForeachFunc feach);

void destroy_hash_table(HashTable table);

#endif
