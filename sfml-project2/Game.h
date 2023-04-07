#pragma once
#include "Player.h"
#include "Slope.h"
#include "Background.h"
#include "Enemy.h"

enum Gamestate {MENU = 0, PLAYING, GAME_OVER};

class Game
{
private:
	//Window variables
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videomode;

	//Player pointer
	Player* player;

	//Background
	Background background;

	//Sloper vector
	std::vector<Slope> slopes;
	Slope slope;
	sf::Clock slopeSpawnTimer;

	//Enemy vector
	std::vector<Enemy> enemies;
	Enemy enemy;
	sf::Clock enemySpawnTimer;

	//Initializer functions
	void initWindow();
	void initPlayer();

	//Containers for mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	void updateEnemyVector();

	void renderEnemyVector();

	void updateSlopeVector();

	void renderSlopeVector();

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

