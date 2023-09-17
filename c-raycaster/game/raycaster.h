#pragma once

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../maths/vector.h"
#include "../maths/angle.h"
#include "../globals.h"
#include "ray.h"

#define RAYS 45
#define RAY_LENGTH 500

typedef struct {
	Ray* rays[RAYS];
	Vector hits[RAYS];
} Raycaster;

Raycaster* Raycaster_Alloc();
void Raycaster_Update(Raycaster* raycaster, const Vector* position, double mouseAngle);
void Raycaster_FreeRays(Raycaster* raycaster);
void Raycaster_RenderView(const Raycaster* raycaster, SDL_Renderer* renderer);
