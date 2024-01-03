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

Rectangle FullNormRectOrigin()
{
    return Rectangle{
        .x = 0.0,
        .y = 0.0,
        .width = 1.0,
        .height = 1.0,
    };
}
