#pragma once

class IEnemy;

class EnemySpawnerController
{
public:
	EnemySpawnerController();

	void Update();

	void Render();

	~EnemySpawnerController();

	void Clear();

private:
	void checkEnemyCollision(const size_t& i);
	void spawnRandomEnemy();
	std::vector<std::shared_ptr<IEnemy>> generateEnemies(bool randomiseSpawnPos = false);
	std::vector<std::shared_ptr<IEnemy>> spawnWave();
private:
	std::vector<std::shared_ptr<IEnemy>> m_Enemies;
	float m_EnemySpawnTimer;
};

