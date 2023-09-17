#include "ray.h"

Ray_Update(Ray* ray, const Vector* position, const Vector* direction) {
	ray->position.x = position->x;
	ray->position.y = position->y;
	ray->direction.x = direction->x;
	ray->direction.y = direction->y;
}