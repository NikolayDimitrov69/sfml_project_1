#pragma once
#include "Animation.h"
#include "Healthbar.h"
#include "Math.h"

class IEnemy
{
protected:
	Healthbar healthbar;
	float maxHP;
	float currentHP;

	Movementstate state;
	Actionstate ac_state;

	Animation frame;
	sf::Sprite sprite;

	float damage;

	float angle;
	sf::Vector2f direction;
	sf::Vector2f randomSpawnpos;

	void updateFrame();
public:
	void setTexure(const sf::Texture& texture);

	bool isFrameFinished() const;

	const Actionstate& getActionstate();

	void setActionState(const Actionstate& state);

	void takeDamage(float);

	const float& getCurrentHP() const;

	const float& dealDamage() const;

	const sf::Vector2f& getPosition() const;

	void randomizeSpawnPosition(const sf::Vector2u& targetSize);

	const sf::FloatRect& getGlobalBounds() const;

	bool outOfBounds(const sf::Vector2u& targetSize);

	virtual bool attackHasHit(const sf::FloatRect& object) = 0;

	virtual void update(const sf::Vector2f& playerpos, const sf::Vector2u& targetSize) = 0;
	
	virtual void render(sf::RenderTarget& target) = 0;

	virtual ~IEnemy() = default;
};

