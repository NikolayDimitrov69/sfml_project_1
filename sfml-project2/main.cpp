#include "precompheaders.h"
#include "Game.h"

int main()
{
	std::cout << "__cplusplus = " << __cplusplus << std::endl;
	std::srand(static_cast<unsigned>(time(NULL)));

	Game game;
	game.initialize();

	while (game.isRunning())
	{
		game.update();

		game.render();
	}
}
