//
// Created by alether on 9/28/23.
//

#include "object.h"

void alObjectRecalculate(Object *self) {
    Matrix matScale = MatrixScale(self->transform.scale.x, self->transform.scale.y, self->transform.scale.z);
    Matrix matRotation = QuaternionToMatrix(self->transform.rotation);
    Matrix matTranslation = MatrixTranslate(self->transform.translation.x,
                                            self->transform.translation.y,
                                            self->transform.translation.z);

    Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
    self->model.transform = matTransform;
}

void alObjectTryRecalculate(Object *self) {
    if (self->hasTransformChanged) {
        alObjectRecalculate(self);
        self->hasTransformChanged = false;
    }
}
