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
	if (slopeSpawnTimer.getElapsedTime().asSeconds() >= 1.5f)
	{
		slope.setRandomVertPos(*window);

		slopes.push_back(slope);

		slopeSpawnTimer.restart();
	}

	for (size_t i = 0; i < slopes.size(); i++)
	{
		slopes[i].update();
		if (slopes[i].getGlobalBounds().top >= window->getSize().y)
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
	if (enemySpawnTimer.getElapsedTime().asSeconds() >= 2.f)
	{
		enemy.setRenderTarget(*window);
		enemy.randomizeSpawnPosition();
		enemy.spawn();
		enemies.push_back(enemy);
		enemySpawnTimer.restart();
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i].updateHoming(player->getPostion());
		if (enemies[i].outOfBounds())
			enemies.erase(enemies.begin() + i);
		else if (enemies[i].getGlobalBounds().intersects(player->getGlobalBounds()))
		{
			player->takeDamage(enemies[i].dealDamage());
			enemies.erase(enemies.begin() + i);
		}
		else if(player->iterateAttackVector(enemies[i].getGlobalBounds())){
			enemies[i].takeDamage(player->dealDamage());
			if (enemies[i].getCurrentHP() <= 0.f)
				enemies.erase(enemies.begin() + i);
		}
	}
}

void Game::renderEnemyVector()
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i].render();
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
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				window->close();
			break;
		}
	}
}

void Game::checkCollision()
{
	if (player->getGlobalBounds().top + player->getGlobalBounds().height >= window->getSize().y)
		player->setPhysicState(Physicstate::ON_GROUND);
	else
		player->setPhysicState(Physicstate::MID_AIR);
}

Game::Game()
{
	enemySpawnTimer.restart();
	slopeSpawnTimer.restart();
	initWindow();
	initPlayer();
}

void Game::update()
{
	pollEvents();
	checkCollision();
	updateSlopeVector();
	updateMousePosition();
	player->updatePlayer(mousePosView);
	updateEnemyVector();
}

void Game::render()
{
	window->clear(sf::Color::White);

	background.render(*window);

	renderSlopeVector();

	renderEnemyVector();

	player->renderPlayer();

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
