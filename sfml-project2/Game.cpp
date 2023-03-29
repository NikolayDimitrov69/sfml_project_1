#include "precompheaders.h"
#include "Game.h"

void Game::initWindow()
{
	videomode = sf::VideoMode(800, 600);
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
}

Game::Game()
{	
	initWindow();
	initPlayer();
}

void Game::update()
{	
	pollEvents();
	updateMousePosition();
	checkCollision();
	player->updatePlayer(mousePosView);
}

void Game::render()
{
	window->clear(sf::Color::White);

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
