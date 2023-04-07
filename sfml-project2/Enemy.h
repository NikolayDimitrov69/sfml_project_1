#pragma once
#include "Animation.h"
#include "Healthbar.h"

constexpr float ENEMY_SPEED = 2.f;
constexpr float ENEMY_DAMAGE = 20.f;

class Enemy
{
private:
	sf::RenderTarget* target;
	
	Healthbar healthbar;
	float maxHP;
	float currentHP;

	Movementstate state;
	Actionstate ac_state;

	Animation frame;
	sf::Texture texture;
	sf::Sprite sprite;
	
	float damage;

	float angle;
	sf::Vector2f direction;
	sf::Vector2f randomSpawnpos;

	void updateFrame();
	
public:
	Enemy();
	
	void takeDamage(float);

	const float& getCurrentHP() const;

	const float& dealDamage() const;

	void setRenderTarget(sf::RenderTarget&);

	void randomizeSpawnPosition();

	void setDirection(const sf::Vector2f&);

	const sf::FloatRect& getGlobalBounds() const;

	bool outOfBounds();

	void spawn();

	void update();

	void render();
};

