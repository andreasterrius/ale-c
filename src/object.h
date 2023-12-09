#ifndef HELLO_C_OBJECT_H
#define HELLO_C_OBJECT_H

#include<raylib.h>
#include<raymath.h>
#include<memory>
#include"rldata.h"

class AlObject {
public:
    Matrix transformMat;
    Transform transform;
    bool hasTransformChanged;

    std::string modelId; // for internally created meshes
    std::string modelPath; // for externally created meshes
    std::shared_ptr<RlModel> model;

    AlObject(Transform transform, std::shared_ptr<RlModel> model);

    void recalculate();

    // If the transform is changed, we should recalculate transform (Transform) -> model.transform (matrix)
    // should be called before rendering/after update
    void tryRecalculate();
};




#endif //HELLO_C_OBJECT_H
