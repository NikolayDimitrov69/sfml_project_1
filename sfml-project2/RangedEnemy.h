#pragma once
#include "IEnemy.h"
#include "Attack.h"

class RangedEnemy : public IEnemy
{
	std::vector<Attack> attacks;
	float attackCooldown;

	float movingTimer;

	void createAttack(const sf::Vector2f& playerpos);
public:
	RangedEnemy();

	void updateAttack();

	void updateAngle(const sf::Vector2f& playerpos);

	void updateDirection();

	void updateTimers();

	virtual void update() override;

	void renderAttack(sf::RenderTarget& target);

	void render(sf::RenderTarget& target);

	bool attackHasHit(const sf::FloatRect& object);

	virtual ~RangedEnemy() = default;
};

