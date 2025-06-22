#include "precompheaders.h"
#include "EnemyConfigComponent.h"

EnemyConfigComponent::EnemyConfigComponent()
	: m_AttackSpeed(0)
	, m_Damage(0)
	, m_Health(0)
	, m_TravelSpeed(0)
	, m_Type(EEnemyType::COUNT)
{
}

void EnemyConfigComponent::Update(const BinaryStruct& data)
{
}
