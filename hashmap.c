//
// Created by Alether on 10/27/2023.
//

#include "hashmap.h"

void alHashMapInit(AlHashMap* map, usize elementSize, usize capacity){
    map->tableSize = capacity + INITIAL_TABLE_SIZE;
    map->size = 0;
    map->elementSize = sizeof(struct AlKeyValuePair);
    map->table = (struct AlKeyValuePair*)malloc(map->tableSize * sizeof(struct AlKeyValuePair));
    for (int i = 0; i < map->tableSize; i++) {
        map->table[i].key = NULL;
    }
}

void alHashMapGrow(AlHashMap *map) {
    int old_size = map->tableSize;
    map->tableSize *= 2;
    map->size = 0;
    struct AlKeyValuePair* old_table = map->table;
    map->table = (struct AlKeyValuePair*)malloc(map->tableSize * sizeof(struct AlKeyValuePair));
    for (int i = 0; i < map->tableSize; i++) {
        map->table[i].key = NULL;
    }

    for (int i = 0; i < old_size; i++) {
        if (old_table[i].key != NULL) {
            alHashMapInsert(map, old_table[i].key, old_table[i].value);
            free(old_table[i].key);
        }
    }

    free(old_table);
}

//TODO: This needs to be a boolean in case of malloc failure
void alHashMapInsert(AlHashMap* map, char* key, void* value) {
    if ((double)map->size / map->tableSize > LOAD_FACTOR) {
        alHashMapGrow(map);
    }

    unsigned int index = hash(key, map->tableSize);
    while (map->table[index].key != NULL) {
        if (strcmp(map->table[index].key, key) == 0) {
            // Free the old value
            free(map->table[index].value);
            map->table[index].value = malloc(map->elementSize);
            memcpy(map->table[index].value, value, map->elementSize);
            return;
        }
        index = (index + 1) % map->tableSize;
    }

    map->table[index].key = strdup(key);
    map->table[index].value = malloc(map->elementSize);
    memcpy(map->table[index].value, value, map->elementSize);
    map->size++;
}

void* alHashMapGet(AlHashMap map, char* key){
    unsigned int index = hash(key, map.tableSize);
    unsigned int initial_index = index;
    while (map.table[index].key != NULL) {
        if (strcmp(map.table[index].key, key) == 0) {
            // Copy the value to the 'result' pointer
            return map.table[index].value; // Key found
        }
        index = (index + 1) % map.tableSize;
        if (index == initial_index) {
            return NULL;
        }
    }
    return NULL;
}


unsigned int hash(char *key, int table_size) {
    int seed = 0;
    unsigned int hashVal = XXH32(key, strlen(key), seed);
    return hashVal % table_size;
}
