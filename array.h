//
// Created by Alether on 10/25/2023.
//

#ifndef HELLO_C_ARRAY_H
#define HELLO_C_ARRAY_H

#include "types.h"
#include <stdlib.h>
#include <string.h>

typedef struct AlArray {
    usize size;
    usize capacity;
    usize elementSize; //sizeof(T)
    void *data;
} AlArray;

bool alArrayInit(AlArray *self, usize elementSize, usize initialCapacity);

bool alArrayPush(AlArray *self, void *data);

bool alArrayPop(AlArray *self);

void* alArrayGet(AlArray self, usize index);

i32 alArraySize(AlArray self);

void alArrayDeinit(AlArray *self);

#endif //HELLO_C_ARRAY_H
