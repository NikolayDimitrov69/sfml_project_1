#pragma once
#include "Player.h"
#include "Slope.h"
#include "Background.h"
#include "Enemy.h"
#include "Pause.h"

constexpr float ENEMY_SPAWN_TIMER = 400.f;
constexpr float SLOPER_SPAWN_TIMER = 225.f;

class Game
{
private:
	//Window variables
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videomode;

	Pause pause_window;

	Gamestate gamestate;

	//Player pointer
	Player* player;

	//Background
	Background background;

	//Sloper vector
	std::vector<Slope> slopes;
	sf::Vector2f previous_pos;
	sf::Texture slope_texture;
	float slopeSpawnTimer;

	//Enemy vector
	std::vector<Enemy> enemies;
	sf::Texture enemy_texture;
	float enemySpawnTimer;

	//Initializer functions
	void initWindow();
	void initPlayer();

	//Containers for mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	void updateEnemyVector();

	void checkEnemyCollision(const size_t& i);

	void renderEnemyVector();

	void updateSlopeVector();

	void renderSlopeVector();

	//Pollevents updater
	void pollEvents();

	void initSpawnSlope();

	//Mouse position updater
	void updateMousePosition();

	//Will check for collision between the player and an object on the screen
	void checkCollision();
public:
	Game();

	//Will render different types of menus if any, based on current game state
	void renderMenu();

	void updateMenu();

	void updatePlayer();

	void update();

	void render();

	bool isRunning() const;

	virtual ~Game();
};

