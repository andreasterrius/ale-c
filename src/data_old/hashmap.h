//
// Created by Alether on 10/27/2023.
//

#ifndef HELLO_C_HASHMAP_H
#define HELLO_C_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>
#include "types.h"

#define INITIAL_TABLE_SIZE 100
#define LOAD_FACTOR 0.7

typedef struct AlKeyValuePair {
    char* key;
    void* value;
} AlKeyValuePair;

typedef struct AlHashMap {
    usize tableSize;
    usize size; // Number of elements in the map
    usize elementSize; //sizeof(T)
    AlKeyValuePair* table;
} AlHashMap;

unsigned int hash(char* key, int table_size);

void alHashMapInit(AlHashMap* map, usize elementSize, usize capacity);

void alHashMapGrow(AlHashMap* map);

void alHashMapInsert(AlHashMap* map, char* key, void* value);

void* alHashMapGet(AlHashMap map, char* key);

void alHashMapDeinit(AlHashMap *map, bool freeContents);


#endif //HELLO_C_HASHMAP_H
