#include "precompheaders.h"
#include "IEnemy.h"
#include "Constants.h"

void IEnemy::updateFrame()
{
	frame.update(state, ac_state);
	sprite.setTextureRect(frame.getCurrentFrame());
}

void IEnemy::setTexure(const sf::Texture& texture)
{
	sprite.setTexture(texture);
	frame.setTextureSize(texture.getSize());
}

bool IEnemy::isFrameFinished() const
{
	return frame.isFinished();
}

const EActionState& IEnemy::getActionstate()
{
	return ac_state;
}

void IEnemy::setActionState(const EActionState& state)
{
	ac_state = state;
}

void IEnemy::takeDamage(float damage)
{
	currentHP -= damage;
}

const float& IEnemy::getCurrentHP() const
{
	return currentHP;
}

const float& IEnemy::dealDamage() const
{
	return damage;
}

const sf::Vector2f& IEnemy::getPosition() const
{
	return sprite.getPosition();
}

void IEnemy::randomizeSpawnPosition(const sf::Vector2u& targetSize)
{
	//Random switch case for wether the enemy will be spawned from left or right side of the screen
	switch (rand() % 2)
	{
	case 0: randomSpawnpos.x = 0.f - sprite.getGlobalBounds().width; sprite.setScale(sprite.getScale().x, -1.f * sprite.getScale().y);
		break;
	case 1: randomSpawnpos.x = static_cast<float>(targetSize.x);
		break;
	default:
		break;
	}
	randomSpawnpos.y = static_cast<float>(rand() % targetSize.y);

	sprite.setPosition(randomSpawnpos);
}

sf::FloatRect IEnemy::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

bool IEnemy::outOfBounds(const sf::Vector2u& targetSize)
{
	if (sprite.getPosition().x > targetSize.x || sprite.getPosition().x < 0 - sprite.getGlobalBounds().width)
	{
		return true;
	}
	return false;
}

bool IEnemy::immunityOver() const
{
	return immunityCoolDown >= ENEMY_MAX_IMMUNITY_TIMER;
}

void IEnemy::resetImmunityTimer()
{
	immunityCoolDown = 0.f;
}
