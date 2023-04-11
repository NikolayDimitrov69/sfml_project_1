#include "precompheaders.h"
#include "Enemy.h"

void Enemy::updateFrame()
{
	frame.update(state, ac_state);
	sprite.setTextureRect(frame.getCurrentFrame());
}

Enemy::Enemy(const sf::Texture& texture)
{
	state = Movementstate::IDLE;
	ac_state = Actionstate::NOT_SHOOTING;
	damage = ENEMY_DAMAGE;
	maxHP = ENEMY_MAX_HEALTH;
	currentHP = maxHP;
	setTexure(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 50, 32));
	sprite.setScale(-3.f, 3.f);
	sprite.setOrigin(sprite.getLocalBounds().width / 1.5f, sprite.getLocalBounds().height / 2.f);
	frame.setDimension(50, 32);
	frame.setIdleSpeed(0.075f);
}

void Enemy::setTexure(const sf::Texture& texture)
{
	sprite.setTexture(texture);
	frame.setTextureSize(texture.getSize());
}

bool Enemy::isFrameFinished() const
{
	return frame.isFinished();
}

const Actionstate& Enemy::getActionstate()
{
	return ac_state;
}

void Enemy::setActionState(const Actionstate& state)
{
	ac_state = state;
}

void Enemy::takeDamage(float damage)
{
	currentHP -= damage;
}

const float& Enemy::getCurrentHP() const
{
	return currentHP;
}

const float& Enemy::dealDamage() const
{
	return damage;
}

void Enemy::randomizeSpawnPosition(const sf::Vector2u& targetSize)
{
	//Random switch case for wether the enemy will be spawned from left or right side of the screen
	switch (rand() % 2)
	{
	case 0: randomSpawnpos.x = 0.f - sprite.getGlobalBounds().width; if (sprite.getScale().y > 0) sprite.setScale(sprite.getScale().x, -1.f * sprite.getScale().y);
		break;
	case 1: randomSpawnpos.x = static_cast<float>(targetSize.x); if (sprite.getScale().y < 0) sprite.setScale(sprite.getScale().x, -1.f * sprite.getScale().y);
		break;
	default:
		break;
	}
	randomSpawnpos.y = static_cast<float>(rand() % targetSize.y);

	sprite.setPosition(randomSpawnpos);
}

void Enemy::setDirection(const sf::Vector2f& playerpos)
{
	direction = normalize(playerpos - sprite.getPosition());
	angle = findAngleCos(sprite.getPosition(), playerpos);
}

const sf::FloatRect& Enemy::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

bool Enemy::outOfBounds(const sf::Vector2u& targetSize)
{
	if (sprite.getPosition().x > targetSize.x || sprite.getPosition().x < 0 - sprite.getGlobalBounds().width)
	{
		return true;
	}
	return false;
}

void Enemy::updateHoming(const sf::Vector2f& playerpos)
{
	//Enemy stops moving when dying
	if (ac_state == Actionstate::NOT_SHOOTING) {
		direction = normalize(playerpos - sprite.getPosition());
		angle = findAngleCos(sprite.getPosition(), playerpos);
		if (playerpos.y > sprite.getPosition().y)
			angle = -angle;
		sprite.setRotation(angle);
		sprite.move(ENEMY_SPEED * direction);
	}

	healthbar.update(sprite, maxHP, currentHP);
	updateFrame();
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(sprite);
	if (ac_state == Actionstate::NOT_SHOOTING)
		healthbar.render(target);
}
