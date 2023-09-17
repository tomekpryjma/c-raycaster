#pragma once

#include <stdio.h>
#include <SDL.h>
#include "globals.h"
#include "game-state.h"
#include "game/level.h"

struct GameCore {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	struct GameState* state;
	Level* level;
};

struct GameCore* GameCore_Alloc();
void GameCore_Init(struct GameCore* gameCore);
void GameCore_Print(char* message);
void GameCore_TearDown(struct GameCore* gameCore);
void GameCore_GameLoop(struct GameCore* gameCore);