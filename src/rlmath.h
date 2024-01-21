//
// Created by Alether on 12/9/2023.
//

#ifndef HELLO_C_RLMATH_H
#define HELLO_C_RLMATH_H

#include"raylib.h"
#include"raymath.h"

Transform TransformOrigin();

Rectangle FullNormRectOrigin();

bool IsPointInsideRect(Rectangle r, Vector2 point);

Vector3 ColorNormalize3(Color);

#endif 
