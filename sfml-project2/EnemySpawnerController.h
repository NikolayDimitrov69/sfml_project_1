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
private:
	std::vector<IEnemy*> m_Enemies;
	float m_EnemySpawnTimer;
};

