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

void Level_DrawWalls(const Level* level, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRects(renderer, level->walls, level->wallCount);
}

void Level_DrawPlayer(const Level* level, SDL_Renderer* renderer) {
	SDL_Rect rect = { level->player.x, level->player.y, 5, 5 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
}

void Level_Raycast(const Level* level, SDL_Renderer* renderer) {
	for (int i = 0; i < RAYS; i++) {
		Ray* ray = level->raycaster->rays[i];
		Vector origin = ray->position;
		float dx = (origin.x + ray->direction.x * RAY_LENGTH) - origin.x;
		float dy = (origin.y + ray->direction.y * RAY_LENGTH) - origin.y;
		int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

		if (steps == 0) continue;

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
				Vector boxHit = {x, y};
				level->raycaster->hits[i] = boxHit;
				hit = true;
			}
		}
	}
}