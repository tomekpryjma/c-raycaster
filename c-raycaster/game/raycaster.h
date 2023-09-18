#pragma once

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../maths/maths.h"
#include "../globals.h"
#include "ray.h"

#define RAYS 90
#define RAY_LENGTH 500

typedef struct {
	Ray* rays[RAYS];
	Vector hits[RAYS];
	float distances[RAYS];
	Vector cameraPlaneDirection;
	Vector cameraPlaneOrigin;
	Vector cameraPlaneA;
	Vector cameraPlaneB;
} Raycaster;

Raycaster* Raycaster_Alloc();
void Raycaster_Update(Raycaster* raycaster, const Vector* position, double lookAngle);
void Raycaster_FreeRays(Raycaster* raycaster);
void Raycaster_RenderView(const Raycaster* raycaster, SDL_Renderer* renderer);

#ifdef DEBUG
void Raycaster_DebugCameraPlane(const Raycaster* raycaster, SDL_Renderer* renderer);
#endif // DEBUG
