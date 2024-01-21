#pragma once
#include "rlmath.h"

enum AlLight_Type {
	AL_LIGHT_TYPE_POINT
};

class AlLight{
public:
	Vector3 position;
	Vector3 colors;
};