#pragma once
#include "IEnemy.h"

constexpr float ENEMY_SPEED = 1.5f;
constexpr float ENEMY_DAMAGE = 20.f;
constexpr float ENEMY_MAX_HEALTH = 100.f;

class Enemy : public IEnemy
{
private:
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
	Enemy(const sf::Texture& texture);
	
	void setTexure(const sf::Texture& texture);

	bool isFrameFinished() const;

	const Actionstate& getActionstate();

	void setActionState(const Actionstate& state);

	void takeDamage(float);

	const float& getCurrentHP() const;

	const float& dealDamage() const;

	void randomizeSpawnPosition(const sf::Vector2u& targetSize);

	const sf::FloatRect& getGlobalBounds() const;

	bool outOfBounds(const sf::Vector2u& targetSize);

	void update(const sf::Vector2f& playerpos);

	void render(sf::RenderTarget& target);
};

