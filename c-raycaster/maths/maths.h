#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#define DEG_TO_RAD (M_PI / 180)

// Vector
typedef struct {
	float x;
	float y;
} Vector;

Vector Vector_DirectionTo(const Vector* vec1, const Vector* vec2);
Vector Vector_Normalize(const Vector* vec);
float Vector_Magnitude(float a, float b);

// Range
int Range_MappedToRange(int input, int a1, int a2, int b1, int b2);