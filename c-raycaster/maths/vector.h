#pragma once

#include <math.h>

typedef struct {
	float x;
	float y;
} Vector;

Vector Vector_DirectionTo(const Vector* vec1, const Vector* vec2);
Vector Vector_Normalize(const Vector* vec);
float Vector_Magnitude(float a, float b);
