//
// Created by Alether on 12/9/2023.
//
#include "rlmath.h"

Transform TransformOrigin() {
    return Transform{
            .translation=Vector3Zero(),
            .rotation=QuaternionIdentity(),
            .scale=Vector3One(),
    };
}