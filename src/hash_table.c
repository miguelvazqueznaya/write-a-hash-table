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
/* frees key and value for an item stored at i
 then frees pointer i*/

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i =0; i< ht->size; i++){
        ht_item* item = ht->items[i];
        if (!item) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}
/* "deletes" a hash table by freeing up each item
 in the hash table by calling ht_del_item in a for loop*/

static int ht_hash(const char *s, const int a, const int m) {
    long hash=0;
    const int len_s = strlen(s);
    for (int i = 0; i< len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

/*this is the hash function. It takes a string as an input
and returns a number between 0 and m, the desired bucket array
length*/
/*the function converts the string to a large integer
 and then reduces the size of the integer by taking its remainder
 mod m. */
 /* the variable a should be a prime larger than size of ASCII 
 alphabet (128) */



 /*Handling collisions: Hash functions map an infinitely large 
 number of inputs to a finite number of outputs Different input 
 keys will map to the same array index. Causing a bucket collision.
 Hash tables must implement some method of dealing with collisions. 
 
 This hash table will handle collisions through a technique called open 
 addressing with double hashing. which makes use of two hash functions
 to calculate the index an item should be stored at after i collisions.*/

 static int ht_get_hash(
    const char * s, const int num_buckets, const int attempt) {
        const int hash_a = ht_hash(s,151, num_buckets);
        const int hash_b = ht_hash(s,163, num_buckets);
        return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
    }
 
 /*the code above gets the hash index for an item that experienced
 a collision by running it though another hash function to prevent 
 2 items from being stored at the same index.  */

 /* *s=== pointer to string which is key to be hashed
    num_buckets === number of spaces in hash (buckets)
    attempt === the current attempt number in case of a collision*/

/* hash_a === stores integer of the result of primary hash
   hash_b === stores integer of result of secondary hash
   
   151 and 163 are primary numbers > 128*/

/* combines the hashes to determine final index. 
Using: original hash_a value. added to attempt number multiplied by
hash_b value +1 (so that it never returns 0 = just hash_a)*/