#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};

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
 ht_hash_table* ht_new(){
    return ht_new_sized(53);
}


ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;

    ht->size= next_prime(ht->base_size);
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

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item) {
        if(cur_item != &HT_DELETED_ITEM){ 
            if(strcmp(cur_item->key, key)==0){
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}

/*this function  inserts a key-value pair into the hash table.*/

/*takes in a pointer ht to a ht_hash_table structure. (where the key-value pair will
be inserted).
 takes in the key value which is a string
 takes in the value which is a string*/

 /*it creates a new ht_item by calling ht_new_item
 then uses gt_get_hash to find the index of the item to be inserted.*/

 /*It then retrieves the current item stored at the  index in the hash table storing
 it to cur_item*/

 /* int i=1 initializes the attempt counter i to 1. This will be used in case
 of rehashing needed.
 The while loop will continue until cur_item is NULL */

 /*it compares the key stored in cur_item to the key of the new item.
 If they match it means the key is already present in the item and we need to replace the existing value with the new one.
 hence ht_del_item of the cur_item
 ht->items[index] = item; places the new item in the hash table at the correct index. replacing the old item. 
 Then the function returns early as it inserted succesfully.*/

 /*if the cur_item isn't NULL but the keys don't match, the function computes a new index using double hashing.
 The index is recalculated for the next probe attempt. The function then checks the next slot in the hash table. 
 Then attempt counter is incremented for the next iteration of the loop. */

 /*Then it sets the ht-items[index] to be that of the new item and increments the counter holding the number
 of items in the hash table*/


 char* ht_search(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item){
        if(item != &HT_DELETED_ITEM){    
            if (strcmp(item->key, key) == 0){
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;  
 }

 /*function to search through the hash table 
 returns a string*/

 /*compares the item being searched's key to the inputted
 key. if they are the same it returns the value
 Else it will go onto the next value.*/



 void ht_delete(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while(item) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) ==0){
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key,ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
 }

 /* "deletes" an item in a bucket*/

 /*in reality instrad of deleting the item it is marked as deleted.
 This prevents any breaking of collision chains that could occur if the 
 item was deleted.*/

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < 53) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}


static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    // ...
}


void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    // ...
}