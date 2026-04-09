#include "precompheaders.h"
#include "Game.h"
#include "Player.h"
#include "RangedEnemy.h"
#include "Enemy.h"
#include "PlayerManagerService.h"
#include "TextureLoader.h"
#include "EnemySpawnerConfig.h"
#include "EnemySpawnerController.h"
#include "GameObjectsLoader.h"
#include "ComponentRules.h"

void Game::initMenus()
{
	auto window = GetGameWindow();
	auto textureLoader = GetConfigLoader().Get<TextureLoader>();
	ReturnUnless(textureLoader);
	go_window.setTexture(*textureLoader->GetTexture("gameover"), 3, 7.f, sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.5f));
	home_window.setTexture(*textureLoader->GetTexture("home"), 1, 5.f, sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f));
	stage_complete_window.setTexture(*textureLoader->GetTexture("stageComplete"), 1, 8.f, sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.5f));
}

void Game::initProgressBar()
{
	auto window = GetGameWindow();
	progressBar.setSize(window->getSize().x / 2.f, 20.f);
	progressBar.setPosition(sf::Vector2f(window->getSize().x/ 4.f, 25));
}

void Game::setupItemEffectBar(ItemEffectBar& ieb, const sf::Texture& texture, float verticalPos)
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
	auto textureLoader = GetConfigLoader().Get<TextureLoader>();
	ReturnUnless(textureLoader);
	auto window = GetGameWindow();

	setupItemEffectBar(player_doubleAttackTimer, *textureLoader->GetTexture("doubleAttack"), window->getSize().y / 3.f);
	setupItemEffectBar(player_boostAttackTimer, *textureLoader->GetTexture("boostAttack"), window->getSize().y / 3.f + textureLoader->GetTexture("boostAttack")->getSize().y * 5.f);
}

void Game::updateMousePosition()
{
	auto window = GetGameWindow();
	mousePosWindow = sf::Mouse::getPosition(*window);
	mousePosView = window->mapPixelToCoords(mousePosWindow);
}


void Game::spawnSlope()
{
	auto textureLoader = GetConfigLoader().Get<TextureLoader>();
	ReturnUnless(textureLoader);
	auto window = GetGameWindow();

	Slope slope(*textureLoader->GetTexture("slope"));
	slope.setFallSpeed(GAME_INIT_SLOPE_FALL_SPEED);
	slope.setScale(sf::Vector2f(3.f, 3.f));
	slope.setRandomVertPos(window->getSize(), previous_pos);
	previous_pos = slope.getPosition();
	slopes.push_back(slope);

	slopeSpawnTimer = 0.f;
}

void Game::updateItemEffectBar()
{
	auto player = GetPlayerObject();
	player_doubleAttackTimer.barTimer.updateStatic(PLAYER_DOUBLE_ATTACK_TIMER, PLAYER_DOUBLE_ATTACK_TIMER - player->getDoubleAttackTimer());
	player_boostAttackTimer.barTimer.updateStatic(PLAYER_BOOST_ATTACK_TIMER, PLAYER_BOOST_ATTACK_TIMER - player->getBoostAttackTimer());
}

void Game::renderItemEffectBar()
{
	auto window = GetGameWindow();
	auto player = GetPlayerObject();
	if (player->getDoubleAttackTimer() <= PLAYER_DOUBLE_ATTACK_TIMER && gamestate == EGameState::Playing)
	{
		player_doubleAttackTimer.barTimer.render(*window);
		window->draw(player_doubleAttackTimer.sprite);
	}
	if (player->getBoostAttackTimer() <= PLAYER_BOOST_ATTACK_TIMER && gamestate == EGameState::Playing)
	{
		player_boostAttackTimer.barTimer.render(*window);
		window->draw(player_boostAttackTimer.sprite);
	}
	
}

void Game::spawnItemAndItemSlope()
{
	auto window = GetGameWindow();
	auto textureLoader = GetConfigLoader().Get<TextureLoader>();
	ReturnUnless(textureLoader);

	Slope slope(*textureLoader->GetTexture("slope"));
	slope.setFallSpeed(0.5f);
	slope.setScale(sf::Vector2f(2.f, 2.f));
	
	Item item;

	switch (rand() % 4)
	{
	case 0: item = Item(*textureLoader->GetTexture("heart"), EItemType::HEAL, 0.5f);
		break;
	case 1: item = Item(*textureLoader->GetTexture("doubleAttack"), EItemType::DOUBLE_ATTACK, 0.5f);
		break;
	case 2: item = Item(*textureLoader->GetTexture("doubleJump"), EItemType::DOUBLE_JUMP, 0.5f);
		break;
	case 3: item = Item(*textureLoader->GetTexture("boostAttack"), EItemType::BOOST_ATTACK, 0.5f);
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


void Game::applyItemEffect(EItemType spec)
{
	auto player = GetPlayerObject();
	switch (spec)
	{
	case EItemType::HEAL: player->heal(20); points += 2;
		break;
	case EItemType::DOUBLE_ATTACK: player->resetDoubleAttTimer(); points += 4;
		break;
	case EItemType::DOUBLE_JUMP: player->gainDoubleJump(); points += 5;
		break;
	case EItemType::BOOST_ATTACK: player->resetBoostTimer(); points += 2;
		break;
	default:
		break;
	}
}

void Game::updateItemsAndItemSlopes()
{
	itemSlopeSpawnTimer += 1.f;
	if (itemSlopeSpawnTimer >= GAME_ITEM_SLOPE_SPAWN_TIMER)
	{
		spawnItemAndItemSlope();
	}

	auto player = GetPlayerObject();

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i].update();
		if (items[i].getGlobalBounds().intersects(player->getGlobalBounds()))
		{
			applyItemEffect(items[i].GetItemType());
			items.erase(items.begin() + i);
		}
	}
}

void Game::renderItemsAndItemSlopes()
{
	auto window = GetGameWindow();
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i].render(*window);
	}
}

void Game::updateSlopeVector()
{
	slopeSpawnTimer += 1.f;	

	if (slopeSpawnTimer >= GAME_SLOPE_SPAWN_TIMER)
	{
		spawnSlope();
	}

	auto player = GetPlayerObject();
	auto window = GetGameWindow();

	for (size_t i = 0; i < slopes.size(); i++)
	{
		slopes[i].update();
		if (slopes[i].getGlobalBounds().top - slopes[i].getGlobalBounds().height * 1.5f >= window->getSize().y)
			slopes.erase(slopes.begin() + i);

		if (slopes[i].getGlobalBounds().intersects(player->getGlobalBounds())) {
			if (player->getGlobalBounds().top + player->getGlobalBounds().height <= slopes[i].getGlobalBounds().top + 5.f)
			{
				player->setPhysicState(EPhysicState::ON_GROUND);
			}
		}
	}

}

void Game::renderSlopeVector()
{
	auto window = GetGameWindow();
	for (size_t i = 0; i < slopes.size(); i++)
	{
		slopes[i].render(*window);
	}
}

void Game::pollEvents()
{
	auto window = GetGameWindow();
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			gamestate = EGameState::Quit;
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape && gamestate == EGameState::Playing) 
				gamestate = EGameState::Paused; 
			else if (event.key.code == sf::Keyboard::Escape && gamestate == EGameState::Paused) 
				gamestate = EGameState::Playing;
			break;
		}
	}
}

void Game::initSpawnSlope()
{
	auto textureLoader = GetConfigLoader().Get<TextureLoader>();
	ReturnUnless(textureLoader);

	auto player = GetPlayerObject();
	Slope slope(*textureLoader->GetTexture("slope"));
	slope.setScale(sf::Vector2f(10.f, 10.f));
	slope.setPostion(sf::Vector2f(player->getGlobalBounds().left - slope.getGlobalBounds().width / 2.7f, 
								  player->getGlobalBounds().top + player->getGlobalBounds().height));
	slope.setFallSpeed(0.4f);
	slopes.push_back(slope);
}

void Game::checkCollision()
{
	auto window = GetGameWindow();
	auto player = GetPlayerObject();
	if (player->getGlobalBounds().top >= window->getSize().y)
		gamestate = EGameState::GameOver;
	else
		player->setPhysicState(EPhysicState::MID_AIR);
}

void Game::initVariables()
{
	currentProgress = 0.f;
	maxProgress = GAME_MAX_PROGRESS;
	points = 0;
	previous_pos.x = -1.f;
	background.setScale(sf::Vector2f(1.f, 0.8f));
	gamestate = EGameState::Home;
	slopeSpawnTimer = 0.f;
	itemSlopeSpawnTimer = 0.f;
	background.Init();
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
	auto player = GetPlayerObject();
	std::stringstream ss;
	ss << "Points: " << points << '\n';
	ss << "Double jumps: " << player->getDoubleJumps() << '\n';
	uiText.setString(ss.str());
}

void Game::renderText()
{
	GetGameWindow()->draw(uiText);
}

void Game::clearVectors()
{
	slopes.clear();
	items.clear();
}

void Game::restartGame()
{
	clearVectors();
	initVariables();
	GetService<PlayerManagerService>()->InitializePlayer();
	initSpawnSlope();
	GetSystemObject()->get<EnemySpawnerController>()->Clear();
}

void Game::initialize()
{
	InitComponentRules();
	GetSingletonInstance<GameObjectsLoader>().Load();
	GetConfigLoader().Initialize();
	GetServiceManager().RegisterNeededServices();
	initFont();
	initText();
	initVariables();
	GetService<PlayerManagerService>()->InitializePlayer();
	initItemEffectBar();
	initSpawnSlope();
	initProgressBar();
	initMenus();
}

void Game::updateMenu()
{
	auto window = GetGameWindow();

	if (gamestate == EGameState::Paused)
		gamestate = pause_window.update(*window, mousePosView);
	if (gamestate == EGameState::GameOver)
		gamestate = go_window.update(mousePosView, window->getSize(), EGameState::GameOver);
	if (gamestate == EGameState::Home)
		gamestate = home_window.update(mousePosView, window->getSize(), EGameState::Home);
	if (gamestate == EGameState::StageComplete)
		gamestate = stage_complete_window.update(mousePosView, window->getSize(), EGameState::StageComplete);
}

void Game::renderMenu()
{
	auto window = GetGameWindow();

	if (gamestate == EGameState::Paused)
		pause_window.render(*window);
	if (gamestate == EGameState::GameOver)
		go_window.render(*window);
	if (gamestate == EGameState::Home)
		home_window.render(*window);
	if (gamestate == EGameState::StageComplete)
		stage_complete_window.render(*window);
}

void Game::updatePlayer()
{
	auto player = GetPlayerObject();
	player->updatePlayer(mousePosView);
	if (player->getCurrentHealth() <= 0) {
		gamestate = EGameState::GameOver;
	}
}

void Game::updateProgress()
{
	currentProgress += GAME_PROGRESSION_PER_FRAME;
	progressBar.updateStatic(maxProgress, currentProgress);
	if (currentProgress >= maxProgress)
	{
		gamestate = EGameState::StageComplete;
	}
}

void Game::update(float delta)
{
	pollEvents();
	updateMenu();
	updateMousePosition();
	updateItemEffectBar();

	if (gamestate == EGameState::Quit)
		GetGameWindow()->close();

	if (gamestate == EGameState::Restart) {
		restartGame();
		gamestate = EGameState::Playing;
	}
	
	if (gamestate == EGameState::Home)
	{
		restartGame();
		gamestate = EGameState::Home;
	}

	if (gamestate == EGameState::Playing)
	{
		updateProgress();
		GetSystemObject()->get<EnemySpawnerController>()->Update();
		checkCollision();
		updateSlopeVector();
		updateItemsAndItemSlopes();
		updatePlayer();
		updateText();
	}

}

void Game::render()
{
	auto window = GetGameWindow();
	window->clear(sf::Color::White);

	background.render(*window);
	
	renderItemEffectBar();

	auto player = GetPlayerObject();

	if (gamestate != EGameState::Home) {
		renderItemsAndItemSlopes();

		renderSlopeVector();

		GetSystemObject()->get<EnemySpawnerController>()->Render();

		player->renderPlayer(*window);
	}
	if (gamestate == EGameState::Playing) {
		renderText();
		progressBar.render(*window);
	}

	renderMenu();

	window->display();
}

bool Game::isRunning() const
{
	auto window = GetGameWindow();
	return window->isOpen();
}

Game::~Game()
{
}
