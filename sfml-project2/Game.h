#pragma once
#include "Player.h"
#include "Slope.h"
#include "Background.h"
#include "Enemy.h"
#include "RangedEnemy.h"
#include "Gameover.h"
#include "Item.h"

constexpr float ENEMY_SPAWN_TIMER = 200.f;
constexpr float SLOPE_SPAWN_TIMER = 225.f;
constexpr float ITEM_SLOPE_SPAWN_TIMER = 1000.f;
constexpr float PROGRESSION_PER_FRAME = 0.01f;

struct ItemEffectBar {
	sf::Sprite sprite;
	Bar barTimer;
};

class Game
{
private:
	sf::Font font;
	sf::Text uiText;

	float currentProgress;
	float maxProgress;

	ItemEffectBar player_doubleAttackTimer;
	ItemEffectBar player_boostAttackTimer;

	Bar progressBar;

	unsigned int points;

	//Window variables
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videomode;

	Pause pause_window;

	Gameover go_window;

	Gameover home_window;

	Gameover stage_complete_window;

	Gamestate gamestate;

	//Player pointer
	Player* player;
	sf::Texture player_texture;

	//Background
	Background background;

	//Sloper vector
	std::vector<Slope> slopes;
	sf::Vector2f previous_pos;
	sf::Texture slope_texture;
	float slopeSpawnTimer;

	sf::Texture gameoverTexture;
	sf::Texture homeTexture;
	sf::Texture stageCompleteTexture;

	//Items things lol
	sf::Texture heartTexture;
	sf::Texture doubleAttTexture;
	sf::Texture doubleJumpTexture;
	sf::Texture boostAttack;
	std::vector<Item> items;
	float itemSlopeSpawnTimer;

	//Enemy vector
	std::vector<IEnemy *> enemies;
	sf::Texture enemy_texture;
	sf::Texture rangedEnemy_texture;
	float enemySpawnTimer;

	//Initializer functions
	void initMenus();
	void initWindow();
	void initPlayer();
	void initProgressBar();
	void setupItemEffectBar(ItemEffectBar& ieb,sf::Texture& texture, float verticalPos);
	void initItemEffectBar();

	//Containers for mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	void updateItemEffectBar();

	void renderItemEffectBar();

	void spawnItemAndItemSlope();

	void spawnRangedEnemy();

	void spawnHomingEnemy();

	void spawnRandomEnemy();

	void updateEnemyVector();

	void checkEnemyCollision(const size_t& i);

	void renderEnemyVector();

	void spawnSlope();

	void updateItemsAndItemSlopes();

	void renderItemsAndItemSlopes();

	void updateSlopeVector();

	void renderSlopeVector();

	//Pollevents updater
	void pollEvents();

	void initSpawnSlope();

	//Mouse position updater
	void updateMousePosition();

	//Will check for collision between the player and an object on the screen
	void checkCollision();
	void initTextures();
	void initVariables();

	void initFont();
	void initText();

	void updateText();

	void renderText();

	void applyItemEffect(Itemspec spec);

	//Clears all vectors
	void clearVectors();

	//Will restart the game by clearing all vectors and calling all initializer functions again
	void restartGame();
public:
	
	Game();

	//Will render different types of menus, based on current game state
	void renderMenu();

	void updateMenu();

	void updatePlayer();

	void updateProgress();

	void update();

	void render();

	bool isRunning() const;

	virtual ~Game();
};

