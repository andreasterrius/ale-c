#ifndef HELLO_C_OBJECT_H
#define HELLO_C_OBJECT_H

#include<raylib.h>
#include<raymath.h>
#include<memory>
#include<optional>
#include"rldata.h"
#include"pbr_shader.h"

class AlObject {
public:
    Matrix transformMat;
    Transform transform;
    bool hasTransformChanged;

    std::string modelPath; // for externally created meshes
    bool isInternal;
    std::shared_ptr<RlModel> model;
    std::optional<AlPbrMaterial> pbrMaterial;

    AlObject(Transform transform, std::shared_ptr<RlModel> model);

    AlObject(Transform transform, std::shared_ptr<RlModel> model, std::shared_ptr<AlPbrShader> pbrShader);

    void recalculate();

    // If the transform is changed, we should recalculate transform (Transform) -> model.transform (matrix)
    // should be called before rendering/after update
    void tryRecalculate();

    void draw();
};




#endif //HELLO_C_OBJECT_H
