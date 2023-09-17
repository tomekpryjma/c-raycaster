#include "raycaster.h"

Raycaster* Raycaster_Alloc() {
	Raycaster* raycaster = malloc(sizeof(Raycaster));

	if (raycaster == NULL) {
		printf("Could not allocate memory for raycaster.");
		exit(1);
		return;
	}

	Ray* rays[RAYS] = {NULL};

	for (int i = 0; i < RAYS; i++) {
		Ray* ray = malloc(sizeof(Ray));
		if (ray == NULL) {
			// Free any rays that have already been created
			for (int i = 0; i < RAYS; i++) {
				if (rays[i] == NULL) continue;
				free(rays[i]);
			}
			printf("Failed to allocate memory for ray index %d", i);
			exit(1);
		}
		ray->position.x = 0;
		ray->position.y = 0;
		ray->direction.x = 0;
		ray->direction.y = 0;
		rays[i] = ray;
	}

	for (int i = 0; i < RAYS; i++) {
		raycaster->rays[i] = rays[i];
	}

	return raycaster;
}


void Raycaster_Update(Raycaster* raycaster, const Vector* position, double mouseAngle) {
	for (int i = 0; i < RAYS; i++) {
		Ray* ray = raycaster->rays[i];
		double xDir = cos(mouseAngle + i * DEG_TO_RAD);
		double yDir = sin(mouseAngle + i * DEG_TO_RAD);
		Vector direction = {xDir, yDir};
		Ray_Update(ray, position, &direction);
	}
}

void Raycaster_RenderView(const Raycaster* raycaster, SDL_Renderer* renderer) {
	for (int i = 0; i < RAYS; i++) {
		Vector vec = raycaster->hits[i];
		
		if (vec.x == 0 && vec.y == 0) continue;

		int width = (WIN_WIDTH / RAYS);
		int x = i * (WIN_WIDTH / RAYS);
		int y = WIN_HEIGHT / 2;
		float dist = sqrt(vec.x * vec.x + vec.y * vec.y);
		float height = dist;

		SDL_Rect rect = {x, y, width, height};

		int color = (dist / 255) * 100;
		SDL_SetRenderDrawColor(renderer, color, color, color, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Raycaster_FreeRays(Raycaster* raycaster) {
	for (int i = 0; i < RAYS; i++) {
		free(raycaster->rays[i]);
	}
}