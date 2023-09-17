#include "game-core.h"

struct GameCore* GameCore_Alloc() {
	struct GameCore* gameCore = malloc(sizeof(struct GameCore));

	if (gameCore == NULL) {
		printf("Could not allocate memory for game core. Goodbye.");
		exit(1);
	}

	return gameCore;
}

void GameCore_Init(struct GameCore* gameCore) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		GameCore_Print(SDL_GetError());
	}

	if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, 0, &gameCore->window, &gameCore->renderer) != 0) {
		printf("%s", SDL_GetError());
		GameCore_TearDown(gameCore);
	}

	gameCore->surface = SDL_GetWindowSurface(gameCore->window);

	if (gameCore->surface == NULL) {
		printf("%s", SDL_GetError());
		GameCore_TearDown(gameCore);
	}

	gameCore->state = GameState_Alloc();
	gameCore->level = Level_Alloc();
	
	Level_Prepare(gameCore->level);
}

void GameCore_Print(char* message) {
	printf("%s", message);
}

void GameCore_GameLoop(struct GameCore* gameCore) {
	SDL_Event e;
	Vector mousePosition = {0,0};
	
	while (gameCore->state->isRunning) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				GameState_SetIsRunning(gameCore->state, false);
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				mousePosition.x = e.motion.x;
				mousePosition.y = e.motion.y;
			}
		}

		double mouseAngle = atan2f(
			mousePosition.y - gameCore->level->player.y,
			mousePosition.x - gameCore->level->player.x
		);
		mouseAngle -= (RAYS / 2) * DEG_TO_RAD;

		Raycaster_Update(gameCore->level->raycaster, &gameCore->level->player, mouseAngle);
		Level_Raycast(gameCore->level, gameCore->renderer);

		SDL_SetRenderDrawColor(gameCore->renderer, 0, 0, 0, 255);
		SDL_RenderClear(gameCore->renderer);
		Level_DrawWalls(gameCore->level, gameCore->renderer);
		Level_DrawPlayer(gameCore->level, gameCore->renderer);
		Raycaster_RenderView(gameCore->level->raycaster, gameCore->renderer);
		SDL_RenderPresent(gameCore->renderer);
	}

}

void GameCore_TearDown(struct GameCore* gameCore) {
	SDL_DestroyRenderer(gameCore->renderer);
	SDL_FreeSurface(gameCore->surface);
	SDL_DestroyWindow(gameCore->window);
	free(gameCore->state);
	Raycaster_FreeRays(gameCore->level->raycaster);
	free(gameCore->level->raycaster);
	free(gameCore->level);

	// Finish up here
	free(gameCore);
	exit(0);
}