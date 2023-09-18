#include "level.h"

const int LEVEL_MAP[LEVEL_ROWS][LEVEL_COLS] = {
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,9,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1},
};

Level* Level_Alloc() {
	Level* level = (Level*)malloc(sizeof(Level));

	if (level == NULL) {
		printf("Could not allocate memory for level.");
		exit(1);
	}

	level->wallCount = 0;
	level->raycaster = Raycaster_Alloc();
	return level;
}

void Level_Prepare(Level* level) {
	// Adjust cell hitboxes
	// int iterations = LEVEL_ROWS * LEVEL_COLS;

	int wallIndex = 0;
	for (int r = 0; r < LEVEL_ROWS; r++) {
		for (int c = 0; c < LEVEL_COLS; c++) {
			int cellValue = LEVEL_MAP[r][c];
			if (cellValue == 0) {
				continue;
			}
			else if (cellValue == 1) {
				SDL_Rect rect = { c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE };
				level->walls[wallIndex] = rect;
				wallIndex++;
				level->wallCount++;
			}
			else if (cellValue == 9) {
				level->player.x = c * CELL_SIZE;
				level->player.y = r * CELL_SIZE;
			}
		}
	}
}

void Level_Raycast(const Level* level) {
	for (int i = 0; i < RAYS; i++) {
		Ray* ray = level->raycaster->rays[i];
		Vector origin = ray->position;
		float dx = (origin.x + ray->direction.x * RAY_LENGTH) - origin.x;
		float dy = (origin.y + ray->direction.y * RAY_LENGTH) - origin.y;
		int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

		if (steps == 0) continue;

		Vector cameraPlaneIntersection;
		float x1 = level->raycaster->cameraPlaneA.x;
		float y1 = level->raycaster->cameraPlaneA.y;
		float x2 = level->raycaster->cameraPlaneB.x;
		float y2 = level->raycaster->cameraPlaneB.y;
		float x3 = origin.x;
		float y3 = origin.y;
		float x4 = origin.x + ray->direction.x * RAY_LENGTH;
		float y4 = origin.y + ray->direction.y * RAY_LENGTH;
		float denominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

		if (denominator == 0) continue;

		float t = (((x1 - x3) * (y3 - y4)) - ((y1 - y3) * (x3 - x4))) / denominator;
		cameraPlaneIntersection.x = x1 + t * (x2 - x1);
		cameraPlaneIntersection.y = y1 + t * (y2 - y1);

		dx /= steps;
		dy /= steps;
		float x = origin.x;
		float y = origin.y;
		bool hit = false;
		int row = 0;
		int col = 0;

		for (int s = 0; s < steps; s++) {
			if (hit) break;

			x += dx;
			y += dy;
			row = floor(y) / CELL_SIZE;
			col = floor(x) / CELL_SIZE;

			if (LEVEL_MAP[row][col] == 1) {
				hit = true;
			}
		}

		Vector boxHit = { x, y };
		float distance = Vector_Magnitude(boxHit.x - cameraPlaneIntersection.x, boxHit.y - cameraPlaneIntersection.y);
		float perpendicularDistance = distance * ray->direction.y;
		level->raycaster->distances[i] = perpendicularDistance;
	}
}

#ifdef DEBUG
void Level_DebugShowCameraPlaneIntesections(const Vector* cameraPlaneIntersection, SDL_Renderer* renderer) {
	SDL_Rect rect = { cameraPlaneIntersection->x, cameraPlaneIntersection->y, 2, 2 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 153, 255);
	SDL_RenderDrawRect(renderer, &rect);
}
void Level_DebugDrawRay(float x, float y, SDL_Renderer* renderer) {
	SDL_Rect rect = {x, y, 2, 2};
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderDrawRect(renderer, &rect);
}
void Level_DebugDrawPlayer(const Level* level, SDL_Renderer* renderer) {
	SDL_Rect rect = { level->player.x, level->player.y, 5, 5 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
}
void Level_DebugDrawWalls(const Level* level, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRects(renderer, level->walls, level->wallCount);
}
#endif // DEBUG