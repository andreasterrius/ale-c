//
// Created by alether on 9/28/23.
//

#include "object.h"

void AlObject::recalculate() {
    Matrix matScale = MatrixScale(this->transform.scale.x, this->transform.scale.y, this->transform.scale.z);
    Matrix matRotation = QuaternionToMatrix(this->transform.rotation);
    Matrix matTranslation = MatrixTranslate(this->transform.translation.x,
                                            this->transform.translation.y,
                                            this->transform.translation.z);

    Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
    this->model.d.transform = matTransform;
}

void AlObject::tryRecalculate() {
    if (this->hasTransformChanged) {
        this->recalculate();
        this->hasTransformChanged = false;
    }
}


AlObject::AlObject(Transform transform, RlModel model, bool hasTransformChanged) :
        transform(transform), model(std::move(model)), hasTransformChanged(hasTransformChanged){
}
