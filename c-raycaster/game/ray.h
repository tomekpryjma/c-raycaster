#pragma once

#include "../maths/vector.h"

typedef struct {
	Vector position;
	Vector direction;
} Ray;

Ray_Update(Ray* ray, const Vector* position, const Vector* direction);