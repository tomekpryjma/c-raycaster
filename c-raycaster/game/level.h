#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "../globals.h"
#include "raycaster.h"

#define LEVEL_ROWS (WIN_HEIGHT / CELL_SIZE)
#define LEVEL_COLS (WIN_WIDTH / CELL_SIZE)
#define CELL_STANDARD 1
#define CELL_EXTENDED_N 2
#define CELL_EXTENDED_E 3
#define CELL_EXTENDED_S 4
#define CELL_EXTENDED_W 5
#define CELL_EXTENDED_NESW 6

typedef struct {
	SDL_Rect walls[LEVEL_ROWS * LEVEL_COLS];
	int wallCount;
	Vector player;
	Raycaster* raycaster;
} Level;

Level* Level_Alloc();
void Level_Prepare(Level* level);
void Level_Raycast(const Level* level, SDL_Renderer* renderer);

#ifdef DEBUG
void Level_DebugDrawWalls(const Level* level, SDL_Renderer* renderer);
void Level_DebugDrawPlayer(const Level* level, SDL_Renderer* renderer);
#endif // DEBUG
