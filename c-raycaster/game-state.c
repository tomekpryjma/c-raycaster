#include "game-state.h"

struct GameState* GameState_Alloc() {
	struct GameState* gameState = malloc(sizeof(struct GameState));

	if (gameState == NULL) {
		printf("Failed to allocate memory for game state. Goodbye.");
		exit(1);
	}

	gameState->isRunning = true;

	return gameState;
}

void GameState_SetIsRunning(struct GameState* gameState, bool isRunning)
{
	gameState->isRunning = isRunning;
}
