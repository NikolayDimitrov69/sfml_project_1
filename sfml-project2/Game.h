#pragma once
#include "Player.h"

class Game
{
private:
	//Window variables
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videomode;

	//Player pointer
	Player* player;

	//Initializer functions
	void initWindow();
	void initPlayer();

	//Pollevents updater
	void pollEvents();
public:
	Game();

	void update();

	void render();

	bool isRunning() const;

	virtual ~Game();
};

