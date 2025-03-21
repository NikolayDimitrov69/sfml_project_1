#pragma once
#include "IEnemy.h"

class Enemy : public IEnemy
{	
public:
	Enemy(const sf::Texture& texture);
	
	void update(const sf::Vector2f& playerpos, const sf::Vector2u& targetSize);

	void render(sf::RenderTarget& renderTarget);

	bool attackHasHit(const sf::FloatRect& object);

	virtual ~Enemy() = default;
};

