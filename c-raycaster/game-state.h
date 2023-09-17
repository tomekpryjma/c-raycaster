#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct GameState {
	bool isRunning;
};

struct GameState* GameState_Alloc();
void GameState_SetIsRunning(struct GameState* gameState, bool isRunning);