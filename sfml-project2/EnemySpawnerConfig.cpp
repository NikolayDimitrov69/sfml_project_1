#include "precompheaders.h"
#include "EnemySpawnerConfig.h"

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