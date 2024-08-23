#include "hash_table.h"

int main() {
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);
}

//creates a hash table and also deletes it. does not run any
// functions on the hash table