#pragma once
#include "IEnemy.h"

class Enemy : public IEnemy
{	
public:
	Enemy();
	
	virtual void update() override;

	void render(sf::RenderTarget& renderTarget);

	bool attackHasHit(const sf::FloatRect& object);

	virtual ~Enemy() = default;
};

