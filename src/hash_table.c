#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item* ht_new_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

/* ht_item* ht_new_item === function that returns pointer to 
 ht_item. (DEFINED IN hash_table.h)*/

 /*The function takes pointers to constant characters
  aka strings. K represents the key and V represents
   the value for the hash table item*/

/*ht_item* i = malloc(sizeof(ht_item)); === a pointer to
a ht_item structure and allocates memory for the size
of one ht_item*/

/*i->key = strdup(k);
  i->value = strdup(v)
   duplicates string k assigning it to key field in 
   ht_item. 
   duplicates string v assigning it to value field in 
   ht_item.*/
 

ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size=53;
    ht->count= 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

/* declare function ht_new that returns pointer to ht_hash_table
structure.
allocate memory for a new ht_hash_table
pointer ht points to new memory 
ht also stores size of the hash table and number of 
items in the hash table.*/

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i =0; i< ht->size; i++){
        ht_item* item = ht->items[i];
        if (!item) {
            ht_del_item(item);
        }
    }
}