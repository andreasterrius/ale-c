#ifndef HELLO_C_OBJECT_H
#define HELLO_C_OBJECT_H

#include<raylib.h>
#include<raymath.h>
#include<memory>
#include"rldata.h"

class AlObject {
public:
    Transform transform;
    bool hasTransformChanged;
    RlModel model;

    AlObject(Transform transform, RlModel model, bool hasTransformChanged);

    void recalculate();

    // If the transform is changed, we should recalculate transform (Transform) -> model.transform (matrix)
    // should be called before rendering/after update
    void tryRecalculate();
};




#endif //HELLO_C_OBJECT_H
