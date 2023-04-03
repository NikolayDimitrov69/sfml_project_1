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

	//Containers for mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Pollevents updater
	void pollEvents();

	//Mouse position updater
	void updateMousePosition();

	//Will check for collision between the player and an object on the screen
	void checkCollision();
public:
	Game();

	void update();

	void render();

	bool isRunning() const;

	virtual ~Game();
};

