#include "maths.h"

//
// Vector
//
Vector Vector_DirectionTo(const Vector* vec1, const Vector* vec2) {
	Vector direction = { vec2->x - vec1->x, vec2->y - vec1->y };
	return Vector_Normalize(&direction);
}

Vector Vector_Normalize(const Vector* vec) {
	Vector normalized = { 0, 0 };
	float magintude = Vector_Magnitude(vec->x, vec->y);

	if (magintude == 0) {
		return normalized;
	}

	normalized.x = vec->x / magintude;
	normalized.y = vec->y / magintude;

	return normalized;
}

float Vector_Magnitude(float a, float b) {
	return sqrtf(a * a + b * b);
}

//
// Range
//
int Range_MappedToRange(int input, int a1, int a2, int b1, int b2) {
	if ((a2 - a1) == 0) return 0;
	return b1 + (((input - a1) * (b2 - b1)) / (a2 - a1));
}