#include "precompheaders.h"
#include "Game.h"

int main()
{
	std::srand(static_cast<unsigned>(time(NULL)));

	Game game;
	game.initialize();

	sf::Clock c;

	while (game.isRunning())
	{
		float deltaTime = c.restart().asSeconds();

		game.update(deltaTime);

		game.render();
	}
}
