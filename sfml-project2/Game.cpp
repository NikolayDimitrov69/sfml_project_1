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
		if (slopes[i].getSlopeTop() >= window->getSize().y)
			slopes.erase(slopes.begin() + i);

		if (slopes[i].getGlobalBounds().intersects(player->getGlobalBounds())) {
			if (player->getGlobalBounds().top + player->getGlobalBounds().height <= slopes[i].getGlobalBounds().top + 5.f)
			{
				player->setPhysicState(Physicstate::ON_GROUND);
			}
		}
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
	if (player->getBottomHitbox() >= window->getSize().y)
		player->setPhysicState(Physicstate::ON_GROUND);
	else
		player->setPhysicState(Physicstate::MID_AIR);
}

Game::Game()
{
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
	
}

void Game::render()
{
	window->clear(sf::Color::White);

	renderSlopeVector();

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
