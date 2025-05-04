#include "precompheaders.h"
#include "EnemySpawnerConfig.h"

EnemySpawnerConfig::EnemySpawnerConfig()
	: m_DefaultSpawnCooldown(0)
{
}

void EnemySpawnerConfig::Update(const json& section)
{
	json spawnRatesArr;
	ReturnUnless(JsonParse(section, "spawnRates", spawnRatesArr));
	ReturnUnless(spawnRatesArr.is_array());

	for (const auto& spawnRateElem : spawnRatesArr)
	{
		int enemyType = 0;
		ContinueUnless(JsonParse(spawnRateElem, "enemyType", enemyType));
		ContinueUnless(IsValidEnum(EEnemyType(enemyType)));
		int weight = 0;
		ContinueUnless(JsonParse(spawnRateElem, "weight", weight));
		m_WeightedSpawnRates.AddUnique((EEnemyType)enemyType, weight);
	}
}

const EnemySpawnRates& EnemySpawnerConfig::GetWeightedSpawnRates() const
{
	return m_WeightedSpawnRates;
}

float EnemySpawnerConfig::GetDefaultSpawnCooldown() const
{
	return m_DefaultSpawnCooldown;
}
