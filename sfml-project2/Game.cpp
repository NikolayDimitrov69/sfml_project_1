#include "precompheaders.h"
#include "Game.h"

void Game::initMenus()
{
	go_window.setTexture(gameoverTexture, 3, 7.f, sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.5f));
	home_window.setTexture(homeTexture, 1, 5.f, sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f));
}

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

void Game::initProgressBar()
{
	progressBar.setSize(window->getSize().x / 2.f, 20.f);
	progressBar.setPosition(sf::Vector2f(window->getSize().x/ 4.f, 25));
}

void Game::setupItemEffectBar(ItemEffectBar& ieb, sf::Texture& texture, float verticalPos)
{
	ieb.sprite.setTexture(texture);
	ieb.sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x / 4.f, texture.getSize().y));
	ieb.sprite.setScale(5.f, 5.f);
	ieb.sprite.setPosition(0.f,verticalPos);
	ieb.barTimer.setPosition(sf::Vector2f(ieb.sprite.getPosition().x +
		ieb.sprite.getGlobalBounds().width,
		ieb.sprite.getPosition().y +
		ieb.sprite.getGlobalBounds().height / 4.f));
	ieb.barTimer.setSize(150.f, 20.f);
}

void Game::initItemEffectBar()
{
	setupItemEffectBar(player_doubleAttackTimer, doubleAttTexture, window->getSize().y / 3.f);
	setupItemEffectBar(player_boostAttackTimer, boostAttack, window->getSize().y / 3.f + boostAttack.getSize().y * 5.f);
}

void Game::updateMousePosition()
{
	mousePosWindow = sf::Mouse::getPosition(*window);
	mousePosView = window->mapPixelToCoords(mousePosWindow);
}


void Game::spawnSlope()
{
	Slope slope(slope_texture);
	slope.setFallSpeed(INIT_SLOPE_FALL_SPEED);
	slope.setScale(sf::Vector2f(3.f, 3.f));
	slope.setRandomVertPos(window->getSize(), previous_pos);
	previous_pos = slope.getPosition();
	slopes.push_back(slope);

	slopeSpawnTimer = 0.f;
}

void Game::updateItemEffectBar()
{
	player_doubleAttackTimer.barTimer.updateStatic(PLAYER_DOUBLE_ATTACK_TIMER, PLAYER_DOUBLE_ATTACK_TIMER - player->getDoubleAttackTimer());
	player_boostAttackTimer.barTimer.updateStatic(PLAYER_BOOST_ATTACK_TIMER, PLAYER_BOOST_ATTACK_TIMER - player->getBoostAttackTimer());
}

void Game::renderItemEffectBar()
{
	if (player->getDoubleAttackTimer() <= PLAYER_DOUBLE_ATTACK_TIMER && gamestate == Gamestate::PLAYING)
	{
		player_doubleAttackTimer.barTimer.render(*window);
		window->draw(player_doubleAttackTimer.sprite);
	}
	if (player->getBoostAttackTimer() <= PLAYER_BOOST_ATTACK_TIMER && gamestate == Gamestate::PLAYING)
	{
		player_boostAttackTimer.barTimer.render(*window);
		window->draw(player_boostAttackTimer.sprite);
	}
	
}

void Game::spawnItemAndItemSlope()
{
	Slope slope(slope_texture);
	slope.setFallSpeed(0.5f);
	slope.setScale(sf::Vector2f(2.f, 2.f));
	
	Item item;

	switch (rand() % 4)
	{
	case 0: item = Item(heartTexture, Itemspec::HEAL, 0.5f);
		break;
	case 1: item = Item(doubleAttTexture, Itemspec::DOUBLE_ATTACK, 0.5f);
		break;
	case 2: item = Item(doubleJumpTexture, Itemspec::DOUBLE_JUMP, 0.5f);
		break;
	case 3: item = Item(boostAttack, Itemspec::BOOST_ATTACK, 0.5f);
		break;
	default:
		break;
	}

	switch (rand() % 2)
	{
	case 0: slope.setPostion(sf::Vector2f(window->getSize().x / 5.f, 0.f));
		break;
	case 1: slope.setPostion(sf::Vector2f(4 * window->getSize().x / 5.f, 0.f));
		break;
	default:
		break;
	}
	item.setPosition(sf::Vector2f(slope.getPosition().x + slope.getGlobalBounds().width / 2.f, slope.getPosition().y - item.getGlobalBounds().height / 2.f));

	items.push_back(item);
	slopes.push_back(slope);

	itemSlopeSpawnTimer = 0.f;
}


void Game::applyItemEffect(Itemspec spec)
{
	switch (spec)
	{
	case HEAL: player->heal(20); points += 2;
		break;
	case DOUBLE_ATTACK: player->resetDoubleAttTimer(); points += 4;
		break;
	case DOUBLE_JUMP: player->gainDoubleJump(); points += 5;
		break;
	case BOOST_ATTACK: player->resetBoostTimer(); points += 2;
		break;
	default:
		break;
	}
}

void Game::updateItemsAndItemSlopes()
{
	itemSlopeSpawnTimer += 1.f;
	if (itemSlopeSpawnTimer >= ITEM_SLOPE_SPAWN_TIMER)
	{
		spawnItemAndItemSlope();
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i].update();
		if (items[i].getGlobalBounds().intersects(player->getGlobalBounds()))
		{
			applyItemEffect(items[i].getItemSpec());
			items.erase(items.begin() + i);
		}
	}
}

void Game::renderItemsAndItemSlopes()
{
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i].render(*window);
	}
}

void Game::updateSlopeVector()
{
	slopeSpawnTimer += 1.f;	

	if (slopeSpawnTimer >= SLOPE_SPAWN_TIMER)
	{
		spawnSlope();
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

void Game::spawnRangedEnemy()
{
	IEnemy* enemy = new RangedEnemy(rangedEnemy_texture);
	enemy->randomizeSpawnPosition(window->getSize());
	enemies.push_back(enemy);
}

void Game::spawnHomingEnemy()
{
	IEnemy* enemy = new Enemy(enemy_texture);
	enemy->randomizeSpawnPosition(window->getSize());
	enemies.push_back(enemy);
}

void Game::spawnRandomEnemy()
{
	int a = rand() % 100;
	if (a > 0 && a <= 30)
	{
		spawnRangedEnemy();
	}
	else if (a > 30 && a < 100)
	{
		spawnHomingEnemy();
	}
	
	enemySpawnTimer = 0.f;
}

void Game::updateEnemyVector()
{
	enemySpawnTimer += 1.f;

	if (enemySpawnTimer >= ENEMY_SPAWN_TIMER)
	{
		spawnRandomEnemy();
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->update(player->getPostion(), window->getSize());
		checkEnemyCollision(i);
	}
}

void Game::checkEnemyCollision(const size_t& i)
{
	if (enemies[i]->outOfBounds(window->getSize())) {
		delete enemies[i];
		enemies.erase(enemies.begin() + i);
	}
	else if (enemies[i]->isFrameFinished()) {
		delete enemies[i];
		enemies.erase(enemies.begin() + i);
	}
	else if (enemies[i]->getActionstate() != Actionstate::DYING && enemies[i]->getGlobalBounds().intersects(player->getGlobalBounds()))
	{
		player->takeDamage(enemies[i]->dealDamage());
		enemies[i]->setActionState(Actionstate::DYING);
	}
	else if (enemies[i]->attackHasHit(player->getGlobalBounds()))
	{
		player->takeDamage(enemies[i]->dealDamage());
	}
	else if (enemies[i]->getActionstate() != Actionstate::DYING && player->attackHasHit(enemies[i]->getGlobalBounds()) && enemies[i]->immunityOver()) {
		enemies[i]->takeDamage(player->dealDamage());
		enemies[i]->resetImmunityTimer();
		if (enemies[i]->getCurrentHP() <= 0) {
			points++;
			currentProgress += 3.f;
			enemies[i]->setActionState(Actionstate::DYING);
		}
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
			if (event.key.code == sf::Keyboard::Escape && gamestate == Gamestate::PLAYING) 
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
	rangedEnemy_texture.loadFromFile("IMAGES/ranged_skull.png");
	heartTexture.loadFromFile("IMAGES/heart.png");
	doubleAttTexture.loadFromFile("IMAGES/double_attack.png");
	doubleJumpTexture.loadFromFile("IMAGES/double_jump.png");
	boostAttack.loadFromFile("IMAGES/boost_attack.png");
	gameoverTexture.loadFromFile("IMAGES/gameover.png");
	homeTexture.loadFromFile("IMAGES/home_screen.png");
}

void Game::initVariables()
{
	currentProgress = 0.f;
	maxProgress = 250.f;
	points = 0;
	previous_pos.x = -1.f;
	background.setScale(sf::Vector2f(1.f, 0.8f));
	gamestate = Gamestate::HOME;
	enemySpawnTimer = 0.f;
	slopeSpawnTimer = 0.f;
	itemSlopeSpawnTimer = 0.f;
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
	ss << "Double jumps: " << player->getDoubleJumps() << '\n';
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
	items.clear();
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
	initItemEffectBar();
	initSpawnSlope();
	initProgressBar();
	initMenus();
}

void Game::updateMenu()
{
	if (gamestate == Gamestate::PAUSED)
		gamestate = pause_window.update(*window, mousePosView);
	if (gamestate == Gamestate::OVER)
		gamestate = go_window.update(mousePosView, window->getSize(), OVER);
	if (gamestate == Gamestate::HOME)
		gamestate = home_window.update(mousePosView, window->getSize(), HOME);
}

void Game::renderMenu()
{
	if (gamestate == Gamestate::PAUSED)
		pause_window.render(*window);
	if (gamestate == Gamestate::OVER)
		go_window.render(*window);
	if (gamestate == Gamestate::HOME)
		home_window.render(*window);
}

void Game::updatePlayer()
{
	player->updatePlayer(mousePosView, window->getSize());
	if (player->getCurrentHealth() <= 0) {
		gamestate = Gamestate::OVER;
	}
}

void Game::updateProgress()
{
	currentProgress += PROGRESSION_PER_FRAME;
	progressBar.updateStatic(maxProgress, currentProgress);
}

void Game::update()
{
	pollEvents();
	updateMenu();
	updateMousePosition();
	updateItemEffectBar();

	if (gamestate == Gamestate::QUIT)
		window->close();

	if (gamestate == Gamestate::RESTART) {
		restartGame();
		gamestate = Gamestate::PLAYING;
	}
	
	if (gamestate == Gamestate::HOME)
	{
		restartGame();
		gamestate = Gamestate::HOME;
	}

	if (gamestate == Gamestate::PLAYING)
	{
		updateProgress();
		checkCollision();
		updateSlopeVector();
		updateItemsAndItemSlopes();
		updatePlayer();
		updateEnemyVector();	
		updateText();
	}

}

void Game::render()
{
	window->clear(sf::Color::White);

	background.render(*window);
	
	renderItemEffectBar();

	if (gamestate != HOME) {
		renderItemsAndItemSlopes();

		renderSlopeVector();

		renderEnemyVector();

		player->renderPlayer(*window);
	}
	if (gamestate == Gamestate::PLAYING) {
		renderText();
		progressBar.render(*window);
	}

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
