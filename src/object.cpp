//
// Created by alether on 9/28/23.
//

#include "object.h"

#include <utility>

void AlObject::recalculate() {
    Matrix matScale = MatrixScale(this->transform.scale.x, this->transform.scale.y, this->transform.scale.z);
    Matrix matRotation = QuaternionToMatrix(this->transform.rotation);
    Matrix matTranslation = MatrixTranslate(this->transform.translation.x,
                                            this->transform.translation.y,
                                            this->transform.translation.z);

    Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
    this->transformMat = matTransform;
}

void AlObject::tryRecalculate() {
    if (this->hasTransformChanged) {
        this->recalculate();
        this->hasTransformChanged = false;
    }
}

AlObject::AlObject(
    Transform transform,
    std::shared_ptr<RlModel> model,
    std::shared_ptr<AlPbrShader> pbrShader) :
    transform(transform),  
    model(std::move(model)),
    hasTransformChanged(false),
    transformMat(MatrixIdentity()),
    pbrMaterial(AlPbrMaterial(std::move(pbrShader)))
{
    recalculate();
    for (int i = 0; i < this->model->d.materialCount; ++i) {
        this->model->d.materials[i].shader = this->pbrMaterial.pbrShader->rlShader.d;
    }
}

void AlObject::draw() {
    this->pbrMaterial.passVariables();

    if (this->pbrMaterial.needPassToMat) {
        for (int i = 0; i < this->model->d.materialCount; ++i) {
            if (this->pbrMaterial.albedoMap.has_value()) {
                this->model->d.materials[i].maps[MATERIAL_MAP_ALBEDO].texture = this->pbrMaterial.albedoMap->get()->d;
            }
        }
        this->pbrMaterial.needPassToMat = false;
    }

    DrawModel(this->model->d, Vector3Zero(), 1.0f, WHITE);
}