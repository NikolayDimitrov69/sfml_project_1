#pragma once
#include "Animation.h"
#include "Healthbar.h"
#include "Math.h"

class IEnemy
{
public:
	virtual void setTexure(const sf::Texture& texture) = 0;

	virtual bool isFrameFinished() const = 0;

	virtual const Actionstate& getActionstate() = 0;

	virtual void setActionState(const Actionstate& state) = 0;

	virtual void takeDamage(float) = 0;

	virtual const float& getCurrentHP() const = 0;

	virtual const float& dealDamage() const = 0;

	virtual void randomizeSpawnPosition(const sf::Vector2u& targetSize) = 0;

	virtual const sf::FloatRect& getGlobalBounds() const = 0;

	virtual bool outOfBounds(const sf::Vector2u& targetSize) = 0;

	virtual void update(const sf::Vector2f& playerpos) = 0;

	virtual void render(sf::RenderTarget& target) = 0;
	
	virtual ~IEnemy() = default;
};

