#ifndef HELLO_C_OBJECT_H
#define HELLO_C_OBJECT_H

#include<raylib.h>
#include<raymath.h>

typedef struct AlObject {
    Transform transform;
    bool hasTransformChanged;
    Model model;
} AlObject;

void alObjectRecalculate(AlObject *self);

// If the transform is changed, we should recalculate transform (Transform) -> model.transform (matrix)
// should be called before rendering/after update
void alObjectTryRecalculate(AlObject *self);

void alObjectDeinit(AlObject *self);

#endif //HELLO_C_OBJECT_H
