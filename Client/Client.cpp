#include <SFML/Graphics.hpp>
#include "InGame.h"
#include "Game.h"
#include "Menu.h"

int main()
{

	Game game;

	//game.PushState(new InGame(&game, 1));
	game.PushState(new Menu(&game));
	game.GameLoop();

	return 0;
}
