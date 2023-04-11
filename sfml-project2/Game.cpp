#include "precompheaders.h"
#include "Game.h"

void Game::initWindow()
{
	videomode = sf::VideoMode(1600, 800);
	window = new sf::RenderWindow(videomode, "Game", sf::Style::Close | sf::Style::Titlebar);
	window->setFramerateLimit(144);
}

void Game::initPlayer()
{
	player = new Player(window);
}

void Game::updateMousePosition()
{
	mousePosWindow = sf::Mouse::getPosition(*window);
	mousePosView = window->mapPixelToCoords(mousePosWindow);
}


void Game::updateSlopeVector()
{
	slopeSpawnTimer += 1.f;
  
	if (slopeSpawnTimer >= SLOPER_SPAWN_TIMER)
	{
		Slope slope(slope_texture);
		slope.setFallSpeed(INIT_SLOPE_FALL_SPEED);
		slope.setScale(sf::Vector2f(3.f, 3.f));
		slope.setRandomVertPos(window->getSize(), previous_pos);
		previous_pos = slope.getPosition();
		slopes.push_back(slope);

		slopeSpawnTimer = 0.f;
	}

	for (size_t i = 0; i < slopes.size(); i++)
	{
		slopes[i].update();
		if (slopes[i].getGlobalBounds().top - slopes[i].getGlobalBounds().height * 1.5f >= window->getSize().y)
			slopes.erase(slopes.begin() + i);

		if (slopes[i].getGlobalBounds().intersects(player->getGlobalBounds())) {
			if (player->getGlobalBounds().top + player->getGlobalBounds().height <= slopes[i].getGlobalBounds().top + 5.f)
			{
				player->setPhysicState(Physicstate::ON_GROUND);
			}
		}
	}

}

void Game::updateEnemyVector()
{
	enemySpawnTimer += 1.f;

	if (enemySpawnTimer >= ENEMY_SPAWN_TIMER)
	{
		Enemy enemy(enemy_texture);
		enemy.randomizeSpawnPosition(window->getSize());
		enemies.push_back(enemy);
		enemySpawnTimer = 0.f;
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i].updateHoming(player->getPostion());
		checkEnemyCollision(i);
	}
}

void Game::checkEnemyCollision(const size_t& i)
{
	if (enemies[i].outOfBounds(window->getSize()))
		enemies.erase(enemies.begin() + i);
	else if (enemies[i].getGlobalBounds().intersects(player->getGlobalBounds()) && enemies[i].getActionstate() != Actionstate::DYING)
	{
		player->takeDamage(enemies[i].dealDamage());
		enemies[i].takeDamage(ENEMY_MAX_HEALTH);
	}
	else if (enemies[i].getActionstate() != Actionstate::DYING && player->iterateAttackVector(enemies[i].getGlobalBounds())) {
		enemies[i].takeDamage(player->dealDamage());
	}
	if (enemies[i].getCurrentHP() <= 0) {
		enemies[i].setActionState(Actionstate::DYING);

		if (enemies[i].isFrameFinished())
			enemies.erase(enemies.begin() + i);
	}
}

void Game::renderEnemyVector()
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i].render(*window);
	}
}

void Game::renderSlopeVector()
{
	for (size_t i = 0; i < slopes.size(); i++)
	{
		slopes[i].render(*window);
	}
}

void Game::pollEvents()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			gamestate = Gamestate::QUIT;
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape && gamestate != Gamestate::PAUSED) 
				gamestate = Gamestate::PAUSED; 
			else if (event.key.code == sf::Keyboard::Escape && gamestate == Gamestate::PAUSED) 
				gamestate = Gamestate::PLAYING;
			break;
		}
	}
}

void Game::initSpawnSlope()
{
	Slope slope(slope_texture);
	slope.setScale(sf::Vector2f(10.f, 10.f));
	slope.setPostion(sf::Vector2f(player->getGlobalBounds().left - slope.getGlobalBounds().width / 2.7f, 
								  player->getGlobalBounds().top + player->getGlobalBounds().height));
	slope.setFallSpeed(0.4f);
	slopes.push_back(slope);
}

void Game::checkCollision()
{
	if (player->getGlobalBounds().top >= window->getSize().y)
		gamestate = Gamestate::OVER;
	else
		player->setPhysicState(Physicstate::MID_AIR);
}

Game::Game()
{
	slope_texture.loadFromFile("IMAGES/platform.jpg");
	previous_pos.x = -1.f;
	background.setTexture("IMAGES/background.jpg");
	background.setScale(sf::Vector2f(1.f, 0.8f));
	gamestate = Gamestate::MENU;
	enemySpawnTimer = 0.f;
	slopeSpawnTimer = 0.f;
	enemy_texture.loadFromFile("IMAGES/skull.png");
	initWindow();	
	initPlayer();	
	initSpawnSlope();
}

void Game::updateMenu()
{
	if (gamestate == Gamestate::PAUSED)
		gamestate = pause_window.update(*window, mousePosView);
}

void Game::renderMenu()
{
	if (gamestate == Gamestate::PAUSED)
		pause_window.render(*window);
}

void Game::updatePlayer()
{
	player->updatePlayer(mousePosView);
	if (player->getCurrentHealth() <= 0) {
		gamestate = Gamestate::OVER;
	}
}

void Game::update()
{
	pollEvents();
	updateMenu();
	updateMousePosition();
	if (gamestate == Gamestate::QUIT)
		window->close();
	
	if (gamestate == Gamestate::PLAYING)
	{
		checkCollision();
		updateSlopeVector();
		updatePlayer();
		updateEnemyVector();
	}
	
}

void Game::render()
{
	window->clear(sf::Color::White);

	background.render(*window);

	renderSlopeVector();

	renderEnemyVector();

	player->renderPlayer();

	renderMenu();
	
	window->display();
}

bool Game::isRunning() const
{
	return window->isOpen();
}

Game::~Game()
{
	delete player;
	delete window;
}
