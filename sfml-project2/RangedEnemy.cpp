#include "precompheaders.h"
#include "RangedEnemy.h"

void RangedEnemy::createAttack(const sf::Vector2f& playerpos)
{
	if (attackCooldown >= RANGED_ENEMY_ATTACK_COOLDOWN)
	{
		attackCooldown = 0.f;
		Attack attack(attackTexture, 10, 10, 4.f, 7.f);
		attack.setShootDir(playerpos, sprite.getPosition());
		attack.changeDirection(playerpos.x < sprite.getPosition().x ? -1 : 1);
		attack.spawn(sprite);
		attacks.push_back(attack);
	}
}

RangedEnemy::RangedEnemy(const sf::Texture& texture)
{
	immunityCoolDown = MAX_IMMUNITY_TIMER;
	attackTexture.loadFromFile("IMAGES/ranged_skull_attack.png");
	attackCooldown = 0.f;
	state = Movementstate::MOVING;
	ac_state = Actionstate::NOT_SHOOTING;
	damage = RANGED_ENEMY_DAMAGE;
	maxHP = RANGED_ENEMY_MAX_HEALTH;
	currentHP = maxHP;
	setTexure(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 50, 32));
	sprite.setScale(-3.f, 3.f);
	sprite.setOrigin(sprite.getLocalBounds().width / 1.5f, sprite.getLocalBounds().height / 2.f);
	frame.setNumberOfFrames(3);
	frame.setDimension(50, 32);
	frame.setIdleSpeed(0.075f);
}

void RangedEnemy::updateAttack(const sf::Vector2u& targetSize)
{
	for (int i = 0; i < attacks.size(); i++)
	{
		attacks[i].update(targetSize);
		if (attacks[i].isOutOfBounds())
		{
			attacks.erase(attacks.begin() + i);
		}
	}
}

void RangedEnemy::updateAngle(const sf::Vector2f& playerpos)
{
	angle = findAngleCos(sprite.getPosition(), playerpos);
	if (playerpos.y > sprite.getPosition().y)
		angle = -angle;
	sprite.setRotation(angle);
}

void RangedEnemy::updateDirection()
{
	direction.x = -std::abs(sprite.getScale().y) / sprite.getScale().y;
	direction.y = 0;
}

void RangedEnemy::updateTimers()
{
	attackCooldown += 1.f;
	immunityCoolDown += 1.f;
	if (movingTimer != -1.f)
		movingTimer += 1.f;
}

void RangedEnemy::update(const sf::Vector2f& playerpos, const sf::Vector2u& targetSize)
{
	updateDirection();

	updateAngle(playerpos);

	updateTimers();

	if (movingTimer >= RANGED_ENEMY_MOVING_TIMER)
	{
		movingTimer = -1.f;
		ac_state = Actionstate::SHOOTING;
		state = Movementstate::IDLE;
	}
	if (ac_state == Actionstate::SHOOTING)
	{
		createAttack(playerpos);
		updateAttack(targetSize);
	}
	if (ac_state == Actionstate::NOT_SHOOTING) {
		sprite.move(RANGED_ENEMY_SPEED * direction);
	}

	healthbar.update(sprite, maxHP, currentHP);
	updateFrame();
}

void RangedEnemy::renderAttack(sf::RenderTarget& target)
{
	for (auto& e : attacks)
	{
		e.render(target);
	}
}

void RangedEnemy::render(sf::RenderTarget& target)
{
	target.draw(sprite);
	if (ac_state != Actionstate::DYING)
	{
		renderAttack(target);
		healthbar.render(target);
	}
}

bool RangedEnemy::attackHasHit(const sf::FloatRect& object)
{
	for (size_t i = 0; i < attacks.size(); i++)
	{
		if (attacks[i].getActionState() == Actionstate::DYING && attacks[i].isFrameFinished()) {
			attacks.erase(attacks.begin() + i);
			return false;
		}
		if (attacks[i].getActionState() != Actionstate::DYING && attacks[i].getGlobalBounds().intersects(object))
		{
			attacks[i].setActionState(Actionstate::DYING);
			return true;
		}
	}
	return false;
}
