#pragma once
#include "IConfigLoader.h"
#include "WeightedList.h"

using EnemySpawnRates = WeightedList<EEnemyType>;

class EnemySpawnerConfig : public IConfigLoader
{
public:
	virtual void Update(const json& section) override;

	const EnemySpawnRates& GetWeightedSpawnRates() const;
private:
	EnemySpawnRates m_WeightedSpawnRates;
};

REGISTER_CONFIG_LOADER(EnemySpawnerConfig, "enemySpawnerConfig");