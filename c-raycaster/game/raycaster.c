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


// TODO: Refactor the raycasting & remove magic numbers.
void Raycaster_Update(Raycaster* raycaster, const Vector* position, double lookAngle) {
	double angleOffset = (RAYS / 2) * DEG_TO_RAD;
	Vector originHeading = { cos(lookAngle) , sin(lookAngle) };
	Vector originAHeading = { cos(lookAngle - angleOffset), sin(lookAngle - angleOffset) };
	Vector originBHeading = { cos(lookAngle + angleOffset), sin(lookAngle + angleOffset) };
	raycaster->cameraPlaneOrigin.x = position->x + originHeading.x * 15;
	raycaster->cameraPlaneOrigin.y = position->y + originHeading.y * 15;
	raycaster->cameraPlaneA.x = position->x + originAHeading.x * 20;
	raycaster->cameraPlaneA.y = position->y + originAHeading.y * 20;
	raycaster->cameraPlaneB.x = position->x + originBHeading.x * 20;
	raycaster->cameraPlaneB.y = position->y + originBHeading.y * 20;

	lookAngle -= angleOffset;

	for (int i = 0; i < RAYS; i++) {
		Ray* ray = raycaster->rays[i];
		double xDir = cos(lookAngle + i * DEG_TO_RAD);
		double yDir = sin(lookAngle + i * DEG_TO_RAD);
		Vector direction = {xDir, yDir};
		Ray_Update(ray, position, &direction);
	}
}

void Raycaster_RenderView(const Raycaster* raycaster, SDL_Renderer* renderer) {
	float maxHeight = 300;
	for (int i = 0; i < RAYS; i++) {
		float distance = raycaster->distances[i];
		distance = distance > maxHeight ? maxHeight : abs(distance);
		int width = (WIN_WIDTH / RAYS);
		int height = maxHeight - abs(distance);
		int x = i * (WIN_WIDTH / RAYS);
		int y = (WIN_HEIGHT / 2) - height / 2;

		SDL_Rect rect = {x, y, width, height };

		int color = Range_MappedToRange(distance, 0, maxHeight, 150, 10);
		SDL_SetRenderDrawColor(renderer, color, color, color, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Raycaster_FreeRays(Raycaster* raycaster) {
	for (int i = 0; i < RAYS; i++) {
		free(raycaster->rays[i]);
	}
}

#ifdef DEBUG
void Raycaster_DebugCameraPlane(const Raycaster* raycaster, SDL_Renderer* renderer) {
	SDL_Rect rect = { raycaster->cameraPlaneOrigin.x, raycaster->cameraPlaneOrigin.y, 5, 5 };
	SDL_Rect rectA = { raycaster->cameraPlaneA.x, raycaster->cameraPlaneA.y, 5, 5 };
	SDL_Rect rectB = { raycaster->cameraPlaneB.x, raycaster->cameraPlaneB.y, 5, 5 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rectA);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &rectB);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, raycaster->cameraPlaneA.x, raycaster->cameraPlaneA.y, raycaster->cameraPlaneB.x, raycaster->cameraPlaneB.y);
}
#endif // DEBUG