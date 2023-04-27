#include "precompheaders.h"
#include "Enemy.h"


Enemy::Enemy(const sf::Texture& texture)
{
	immunityCoolDown = MAX_IMMUNITY_TIMER;
	state = Movementstate::IDLE;
	ac_state = Actionstate::NOT_SHOOTING;
	damage = ENEMY_DAMAGE;
	maxHP = ENEMY_MAX_HEALTH;
	currentHP = maxHP;
	setTexure(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 50, 32));
	sprite.setScale(-3.f, 3.f);
	sprite.setOrigin(sprite.getLocalBounds().width / 1.5f, sprite.getLocalBounds().height / 2.f);
	frame.setNumberOfFrames(3);
	frame.setDimension(50, 32);
	frame.setIdleSpeed(0.075f);
}

void Enemy::update(const sf::Vector2f& playerpos, const sf::Vector2u& targetSize)
{
	immunityCoolDown += 1.f;

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

void Enemy::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(sprite);
	if (ac_state == Actionstate::NOT_SHOOTING)
	{
		healthbar.render(renderTarget);
	}
}

bool Enemy::attackHasHit(const sf::FloatRect& object)
{
	return false;
}

