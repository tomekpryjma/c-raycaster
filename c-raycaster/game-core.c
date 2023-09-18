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
	int lookDegrees = 0;
	
	while (gameCore->state->isRunning) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				GameState_SetIsRunning(gameCore->state, false);
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				lookDegrees += e.motion.xrel;
			}
		}

		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		if (lookDegrees > 360) {
			lookDegrees = 0;
		}
		else if(lookDegrees < 0) {
			lookDegrees = 360;
		}

		double lookAngle = lookDegrees * DEG_TO_RAD;

		// TODO: Fix the movement & add collisions with walls
		// TODO: Also - the perspective is a bit buggy still, no massive fisheye effect, but still work to be done there.
		if (keyboardState[SDL_SCANCODE_W]) {
			gameCore->level->player.x += cos(lookAngle) * 0.05;
			gameCore->level->player.y += sin(lookAngle) * 0.05;
		}
		else if (keyboardState[SDL_SCANCODE_S]) {
			gameCore->level->player.x -= cos(lookAngle) * 0.05;
			gameCore->level->player.y -= sin(lookAngle) * 0.05;
		}

		Raycaster_Update(gameCore->level->raycaster, &gameCore->level->player, lookAngle);

		SDL_SetRenderDrawColor(gameCore->renderer, 0, 0, 0, 255);
		SDL_RenderClear(gameCore->renderer);
		Level_Raycast(gameCore->level, gameCore->renderer);
		Raycaster_RenderView(gameCore->level->raycaster, gameCore->renderer);
		
#ifdef DEBUG
		Level_DebugDrawPlayer(gameCore->level, gameCore->renderer);
		Level_DebugDrawWalls(gameCore->level, gameCore->renderer);
#endif // DEBUG

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