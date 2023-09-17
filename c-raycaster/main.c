#include "game-core.h"

int main() {
	struct GameCore* gameCore = GameCore_Alloc();
	GameCore_Init(gameCore);
	GameCore_GameLoop(gameCore);
	GameCore_TearDown(gameCore);
	return 0;
}