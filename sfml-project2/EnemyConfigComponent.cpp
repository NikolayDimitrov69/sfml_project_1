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
	m_Health = data.Get<int>("Health");
	m_Damage = data.Get<int>("Damage");
	m_AttackSpeed = data.Get<int>("AttackSpeed");
	m_TravelSpeed = data.Get<int>("TravelSpeed");
	m_Type = EEnemyType(data.Get<int>("EnemyType"));
}
