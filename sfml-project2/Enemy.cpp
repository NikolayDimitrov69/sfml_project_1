#include "precompheaders.h"
#include "Enemy.h"

void Enemy::updateFrame()
{
	frame.update(state, ac_state);
	sprite.setTextureRect(frame.getCurrentFrame());
}

Enemy::Enemy()
{
	state = Movementstate::IDLE;
	ac_state = Actionstate::NOT_SHOOTING;
	damage = ENEMY_DAMAGE;
	maxHP = 100.f;
	currentHP = maxHP;
	texture.loadFromFile("IMAGES/skull.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 50, 32));
	sprite.setScale(-3.f, 3.f);
	sprite.setOrigin(sprite.getLocalBounds().width / 1.5f, sprite.getLocalBounds().height / 2.f);
	frame.setDimension(50, 32);
	frame.setIdleSpeed(0.075f);
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

void Enemy::setRenderTarget(sf::RenderTarget& newtarget)
{
	target = &newtarget;
}

void Enemy::randomizeSpawnPosition()
{
	//Random switch case for wether the enemy will be spawned from left or right side of the screen
	switch (rand() % 2)
	{
	case 0: randomSpawnpos.x = 0.f - sprite.getGlobalBounds().width; if (sprite.getScale().y > 0) sprite.setScale(sprite.getScale().x, -1.f * sprite.getScale().y);
		break;
	case 1: randomSpawnpos.x = static_cast<float>(target->getSize().x); if (sprite.getScale().y < 0) sprite.setScale(sprite.getScale().x, -1.f * sprite.getScale().y);
		break;
	default:
		break;
	}
	randomSpawnpos.y = static_cast<float>(rand() % target->getSize().y);
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

bool Enemy::outOfBounds()
{
	if (sprite.getPosition().x > target->getSize().x || sprite.getPosition().x < 0 - sprite.getGlobalBounds().width)
	{
		return true;
	}
	return false;
}

void Enemy::spawn()
{
	sprite.setPosition(randomSpawnpos);
}

void Enemy::updateHoming(const sf::Vector2f& playerpos)
{
	//Movement unit vector
	direction = normalize(playerpos - sprite.getPosition());
	

	angle = findAngleCos(sprite.getPosition(), playerpos);
	if (playerpos.y > sprite.getPosition().y)
		angle = -angle;

	
	sprite.setRotation(angle);



	sprite.move(ENEMY_SPEED * direction);
	healthbar.update(sprite, maxHP, currentHP);
	updateFrame();
}

void Enemy::render()
{
	target->draw(sprite);
	healthbar.render(*target);
}
