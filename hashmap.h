#ifndef SHOCK_HASHMAP_H
#define SHOCK_HASHMAP_H

#define HASHMAP_SIZE 256

#include "stddef.h"

typedef struct {
    char key[128]; //It is enough to hold POST key names
    char value[256];
} shock_hash_entry_t;

typedef struct {
    size_t length;
    shock_hash_entry_t entries[HASHMAP_SIZE];
} shock_hashmap_t;

shock_hashmap_t shock_hashmap_init();
int shock_hashmap_put(shock_hashmap_t* map, char* key, char* value);
char* shock_hashmap_get(shock_hashmap_t* map, char* key);
size_t shock_hashmap_exists(shock_hashmap_t* map, char* key);

#endif // SHOCK_HASHMAP_H
