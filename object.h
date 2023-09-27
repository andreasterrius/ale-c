#ifndef HELLO_C_OBJECT_H
#define HELLO_C_OBJECT_H

#include<raylib.h>
#include<raymath.h>

typedef struct Object {
    Transform transform;
    bool hasTransformChanged;
    Model model;
} Object;

void alObjectRecalculate(Object *self);

// If the transform is changed, we should recalculate transform (Transform) -> model.transform (matrix)
// should be called before rendering/after update
void alObjectTryRecalculate(Object *self);

#endif //HELLO_C_OBJECT_H
