#include "hashmap.h"
#include "assert.h"

shock_hashmap_t shock_hashmap_init()
{
    shock_hashmap_t map;
    map.length = 0;
    return map;
}

int shock_hashmap_put(shock_hashmap_t* map, char* key, char* value)
{
    assert(map != NULL);

    size_t idx = shock_hashmap_exists(map, key);
    if (idx == -1)  {
        shock_hash_entry_t entry;
        strcpy(entry.key, key);
        strcpy(entry.value, value);
        map->entries[map->length] = entry;
        map->length = map->length + 1;
    } else {
        strcpy(map->entries[idx].value, value);
    }
}

size_t shock_hashmap_exists(shock_hashmap_t* map, char* key)
{
    assert(map != NULL);

    for (int i =0;i<map->length;i++) {
        if (strcmp(map->entries[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

char* shock_hashmap_get(shock_hashmap_t* map, char* key)
{
    assert(map != NULL);

    for (int i =0;i<map->length;i++) {
        if (strcmp(map->entries[i].key, key) == 0) {
            return map->entries[i].value;
        }
    }
}
