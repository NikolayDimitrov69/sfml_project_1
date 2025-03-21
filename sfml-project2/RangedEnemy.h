#pragma once
#include "IEnemy.h"
#include "Attack.h"

class RangedEnemy : public IEnemy
{
	std::vector<Attack> attacks;
	float attackCooldown;

	float movingTimer;

	sf::Texture attackTexture;

	void createAttack(const sf::Vector2f& playerpos);
public:
	RangedEnemy(const sf::Texture& texture);

	void updateAttack(const sf::Vector2u& targetSize);

	void updateAngle(const sf::Vector2f& playerpos);

	void updateDirection();

	void updateTimers();

	void update(const sf::Vector2f& playerpos, const sf::Vector2u& targetSize);

	void renderAttack(sf::RenderTarget& target);

	void render(sf::RenderTarget& target);

	bool attackHasHit(const sf::FloatRect& object);

	virtual ~RangedEnemy() = default;
};

