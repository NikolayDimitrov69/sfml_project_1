#pragma once
#include "IConfigLoader.h"
#include "WeightedList.h"

using EnemySpawnRates = WeightedList<EEnemyType>;

class EnemySpawnerConfig : public IConfigLoader
{
public:
	EnemySpawnerConfig();

	virtual void Update(const json& section) override;

	const EnemySpawnRates& GetWeightedSpawnRates() const;

	float GetDefaultSpawnCooldown() const;
private:
	EnemySpawnRates m_WeightedSpawnRates;
	float			m_DefaultSpawnCooldown;
};

REGISTER_CONFIG_LOADER(EnemySpawnerConfig, "enemySpawnerConfig");