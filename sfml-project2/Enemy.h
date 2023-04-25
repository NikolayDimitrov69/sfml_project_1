#pragma once
#include "IEnemy.h"

constexpr float ENEMY_SPEED = 1.5f;
constexpr float ENEMY_DAMAGE = 20.f;
constexpr float ENEMY_MAX_HEALTH = 100.f;

class Enemy : public IEnemy
{	
public:
	Enemy(const sf::Texture& texture);
	
	void update(const sf::Vector2f& playerpos, const sf::Vector2u& targetSize);

	void render(sf::RenderTarget& renderTarget);

	bool attackHasHit(const sf::FloatRect& object);

	virtual ~Enemy() = default;
};

