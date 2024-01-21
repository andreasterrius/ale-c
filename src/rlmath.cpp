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

bool IsPointInsideRect(Rectangle r, Vector2 point)
{
    if (r.x < point.x && point.x < r.x + r.width &&
        r.y < point.y && point.y < r.y + r.height) {
        return true;
    }
    return false;
}

Vector3 ColorNormalize3(Color c) {
    Vector4 a = ColorNormalize(c);
    return Vector3{
        .x = a.x,
        .y = a.y,
        .z = a.z,
    };
}
