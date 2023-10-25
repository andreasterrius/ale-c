//
// Created by Alether on 10/25/2023.
//

#ifndef HELLO_C_ARRAY_H
#define HELLO_C_ARRAY_H

#include "types.h"
#include <stdlib.h>
#include <string.h>

typedef struct AlArray {
    i32 size;
    i32 capacity;
    i32 elementSize;
    void **data;
} AlArray;

bool alArrayInit(AlArray *self, i32 elementSize, i32 initialCapacity) {
    self->size = 0;
    self->elementSize = elementSize;
    self->capacity = initialCapacity;
    self->data = malloc(initialCapacity * elementSize);
    if (self->data == NULL) {
        return false;
    }
    return true;
}

bool alArrayPush(AlArray *self, void *data) {
    if (self->size >= self->capacity) {
        self->capacity *= 2;
        self->data = realloc(self->data, self->capacity * self->elementSize);
        if (self->data == NULL) {
            return false;
        }
    }

    char *dest = (char *)self->data + (self->size * self->elementSize);
    memcpy(dest, data, self->elementSize);
    self->size += 1;
    return true;
}

void alArrayDeinit(AlArray *self, bool freeContents) {
    if (freeContents) {
        for (int i = 0; i < self->size; ++i) {
            free(self->data[i]);
        }
    }
    if (self->data)
        free(self->data);
}

#endif //HELLO_C_ARRAY_H
