#include "precompheaders.h"
#include "EnemySpawnerController.h"
#include "IEnemy.h"
#include "Player.h"
#include "EnemySpawnerConfig.h"
#include "Enemy.h"
#include "RangedEnemy.h"

EnemySpawnerController::EnemySpawnerController()
	: m_EnemySpawnTimer(0)
{
}

void EnemySpawnerController::Update()
{
	m_EnemySpawnTimer += 1.f;

	if (m_EnemySpawnTimer >= ENEMY_SPAWN_TIMER)
	{
		spawnRandomEnemy();
		m_EnemySpawnTimer = 0;
	}

	auto player = GetPlayerObject();

	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->update();
		checkEnemyCollision(i);
	}
}

void EnemySpawnerController::Render()
{
	auto window = GetGameWindow();
	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->render(*window);
	}
}

EnemySpawnerController::~EnemySpawnerController()
{
	Clear();
}

void EnemySpawnerController::Clear()
{
	m_Enemies.clear();
}

void EnemySpawnerController::checkEnemyCollision(const size_t& i)
{
	auto window = GetGameWindow();
	auto player = GetPlayerObject();
	if (m_Enemies[i]->outOfBounds(window->getSize())) {
		m_Enemies.erase(m_Enemies.begin() + i);
	}
	else if (m_Enemies[i]->isFrameFinished()) {
		m_Enemies.erase(m_Enemies.begin() + i);
	}
	else if (m_Enemies[i]->getActionstate() != EActionState::DYING && m_Enemies[i]->getGlobalBounds().intersects(player->getGlobalBounds()))
	{
		player->takeDamage(m_Enemies[i]->dealDamage());
		m_Enemies[i]->setActionState(EActionState::DYING);
	}
	else if (m_Enemies[i]->attackHasHit(player->getGlobalBounds()))
	{
		player->takeDamage(m_Enemies[i]->dealDamage());
	}
	else if (m_Enemies[i]->getActionstate() != EActionState::DYING && player->attackHasHit(m_Enemies[i]->getGlobalBounds()) && m_Enemies[i]->immunityOver()) {
		m_Enemies[i]->takeDamage(player->dealDamage());
		m_Enemies[i]->resetImmunityTimer();
		if (m_Enemies[i]->getCurrentHP() <= 0) {
			m_Enemies[i]->setActionState(EActionState::DYING);
		}
	}
}

void EnemySpawnerController::spawnRandomEnemy()
{
	auto enemy = generateEnemies(true);
	m_Enemies.insert(m_Enemies.end(), enemy.begin(), enemy.end());
}

std::vector<std::shared_ptr<IEnemy>> EnemySpawnerController::generateEnemies(bool randomiseSpawnPos)
{
	std::vector<std::shared_ptr<IEnemy>> enemies;

	auto cfg = GetConfigLoader().Get<EnemySpawnerConfig>();
	ReturnUnless(cfg, (enemies));

	std::shared_ptr<IEnemy> enemy;

	switch (cfg->GetWeightedSpawnRates().GetRandomItem())
	{
	case EEnemyType::Homing:
	{
		enemy.reset(new Enemy());
		if (randomiseSpawnPos)
		{
			enemy->randomizeSpawnPosition();
		}
		enemies.push_back(enemy);
		break;
	}
	case EEnemyType::Ranged:
	{
		enemy.reset(new RangedEnemy());
		if (randomiseSpawnPos)
		{
			enemy->randomizeSpawnPosition();
		}
		enemies.push_back(enemy);
		break;
	}
	case EEnemyType::Wave:
	{
		return spawnWave();
		break;
	}
	default:
		assert(false && "unkown type of enemy");
	}

	return (enemies);
}

std::vector<std::shared_ptr<IEnemy>> EnemySpawnerController::spawnWave()
{
	std::vector<std::shared_ptr<IEnemy>> enemies;
	enemies.reserve(10);

	for (size_t i = 0; i < 10; i++)
	{
		std::shared_ptr<IEnemy> enemy;
		enemy.reset(new RangedEnemy());
		enemy->randomizeSpawnPosition();
		enemies.push_back(enemy);
	}

	return (enemies);
}
