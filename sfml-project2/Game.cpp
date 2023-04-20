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
	if (player != nullptr)
	{
		delete player;
	}
	player = new Player(window->getSize(), player_texture);
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
		Enemy* enemy = new Enemy(enemy_texture);
		enemy->randomizeSpawnPosition(window->getSize());
		enemies.push_back(enemy);
		enemySpawnTimer = 0.f;
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->update(player->getPostion());
		checkEnemyCollision(i);
	}
}

void Game::checkEnemyCollision(const size_t& i)
{
	if (enemies[i]->outOfBounds(window->getSize()))
		enemies.erase(enemies.begin() + i);
	else if (enemies[i]->getGlobalBounds().intersects(player->getGlobalBounds()) && enemies[i]->getActionstate() != Actionstate::DYING)
	{
		player->takeDamage(enemies[i]->dealDamage());
		enemies[i]->setActionState(Actionstate::DYING);
	}
	else if (enemies[i]->getActionstate() != Actionstate::DYING && player->iterateAttackVector(enemies[i]->getGlobalBounds())) {
		enemies[i]->takeDamage(player->dealDamage());
		if (enemies[i]->getCurrentHP() <= 0) {
			points++;
			enemies[i]->setActionState(Actionstate::DYING);
		}
	}
	if (enemies[i]->isFrameFinished()) {
		delete enemies[i];
		enemies.erase(enemies.begin() + i);
	}
}

void Game::renderEnemyVector()
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->render(*window);
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

void Game::initTextures()
{
	player_texture.loadFromFile("IMAGES/megaman.png");
	slope_texture.loadFromFile("IMAGES/platform.jpg");
	background.setTexture("IMAGES/background.jpg");
	enemy_texture.loadFromFile("IMAGES/skull.png");
}

void Game::initVariables()
{
	points = 0;
	previous_pos.x = -1.f;
	background.setScale(sf::Vector2f(1.f, 0.8f));
	gamestate = Gamestate::PAUSED;
	enemySpawnTimer = 0.f;
	slopeSpawnTimer = 0.f;
}

void Game::initFont()
{
	if (!font.loadFromFile("Fonts/EA Sports Covers SC 1.5.ttf")) {
		std::cerr << "ERROR::GAME::INITFONT::Failed to load font!\n";
	}
}

void Game::initText()
{
	uiText.setFont(font);
	uiText.setFillColor(sf::Color::Black);
	uiText.setCharacterSize(48);
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << points << '\n';
	uiText.setString(ss.str());
}

void Game::renderText()
{
	window->draw(uiText);
}

void Game::clearVectors()
{
	enemies.clear();
	slopes.clear();
}

void Game::restartGame()
{
	clearVectors();
	initVariables();
	initPlayer();
	initSpawnSlope();
}

Game::Game()
{	
	initWindow();	
	initTextures();
	initFont();
	initText();
	initVariables();
	initPlayer();	
	initSpawnSlope();
}

void Game::updateMenu()
{
	if (gamestate == Gamestate::PAUSED)
		gamestate = pause_window.update(*window, mousePosView);
	if (gamestate == Gamestate::OVER)
		go_window.update(mousePosView, window->getSize());
}

void Game::renderMenu()
{
	if (gamestate == Gamestate::PAUSED)
		pause_window.render(*window);
	if (gamestate == Gamestate::OVER)
		go_window.render(*window);
}

void Game::updatePlayer()
{
	player->updatePlayer(mousePosView, window->getSize());
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

	if (gamestate == Gamestate::RESTART)
		restartGame();
	
	if (gamestate == Gamestate::PLAYING)
	{
		checkCollision();
		updateSlopeVector();
		updatePlayer();
		updateEnemyVector();	
		updateText();
	}

}

void Game::render()
{
	window->clear(sf::Color::White);

	background.render(*window);

	renderSlopeVector();

	renderEnemyVector();

	player->renderPlayer(*window);

	if(gamestate !=  Gamestate::MENU)
		renderText();

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
