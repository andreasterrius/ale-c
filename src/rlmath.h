//
// Created by Alether on 12/9/2023.
//

#ifndef HELLO_C_RLMATH_H
#define HELLO_C_RLMATH_H

#include"raylib.h"
#include"raymath.h"

Transform TransformOrigin() {
    return Transform{
            .translation=Vector3Zero(),
            .rotation=QuaternionIdentity(),
            .scale=Vector3One(),
    };
}

#endif //HELLO_C_RLMATH_H