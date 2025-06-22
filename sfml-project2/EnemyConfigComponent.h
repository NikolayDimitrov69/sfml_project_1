#pragma once
#include "IComponent.h"

class EnemyConfigComponent : public IComponent
{
public:
	explicit EnemyConfigComponent();

	virtual void Update(const BinaryStruct& data) override;
private:

private:
	EEnemyType m_Type;
	int m_Health;
	int m_Damage;
	int m_TravelSpeed;
	int m_AttackSpeed;
};

REGISTER_COMPONENT(EnemyConfigComponent, "EnemyConfig");