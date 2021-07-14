#include "game.cpp"

int main(void) {

    Game *game = new Game();

    game->startGame();
    delete game;

	return 0;
}