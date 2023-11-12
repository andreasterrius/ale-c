//
// Created by Alether on 10/26/2023.
//
#include "array.h"

bool alArrayInit(AlArray *self, usize elementSize, usize initialCapacity) {
    self->size = 0;
    self->elementSize = elementSize;
    self->capacity = initialCapacity;
    self->data = malloc(initialCapacity * elementSize);
    if (self->data == NULL) {
        return false;
    }
    return true;
}

// This creates a copy of data in the allocated memory block of the array.
// *data is safe to be free-d after this call (non-owning)
bool alArrayPush(AlArray *self, void *data) {
    if (self->size >= self->capacity) {
        usize newCapacity = self->capacity * 2;
        if(newCapacity == 0) newCapacity += self->elementSize;
        void *newData = realloc(self->data, newCapacity * self->elementSize);
        if (newData == NULL) {
            return false;
        }
        self->data = newData;
        self->capacity = newCapacity;
    }

    char *dest = (char *)self->data + (self->size * self->elementSize);
    memcpy(dest, data, self->elementSize);
    self->size += 1;
    return true;
}

bool alArrayPop(AlArray *self) {
    if (self->size > 0) {
        self->size--;
        return true;
    }
    return false;
}

i32 alArraySize(AlArray self) {
    return self.size;
}

void* alArrayGet(AlArray self, usize index) {
    if (index >= self.size) {
        return NULL; // Index out of bounds
    }
    return (char*)self.data + index * self.elementSize;
}

void alArrayDeinit(AlArray *self) {
    free(self->data);
}
