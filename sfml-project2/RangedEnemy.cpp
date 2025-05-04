#include "precompheaders.h"
#include "RangedEnemy.h"
#include "Constants.h"
#include "TextureLoader.h"
#include "Player.h"

void RangedEnemy::createAttack(const sf::Vector2f& playerpos)
{
	if (attackCooldown >= ENEMY_RANGED_ATTACK_COOLDOWN)
	{
		attackCooldown = 0.f;
		auto texture = GetConfigLoader().Get<TextureLoader>()->GetTexture("rangedSkullAttack");
		Attack attack(*texture, 10, 10, 4.f, 7.f);
		attack.setShootDir(playerpos, sprite.getPosition());
		attack.changeDirection(playerpos.x < sprite.getPosition().x ? -1 : 1);
		attack.spawn(sprite);
		attacks.push_back(attack);
	}
}

RangedEnemy::RangedEnemy()
{
	immunityCoolDown = ENEMY_MAX_IMMUNITY_TIMER;
	attackCooldown = 0.f;
	state = EMovementState::MOVING;
	ac_state = EActionState::NOT_SHOOTING;
	damage = ENEMY_RANGED_DAMAGE;
	maxHP = ENEMY_RANGED_MAX_HEALTH;
	currentHP = maxHP;
	auto texture = GetConfigLoader().Get<TextureLoader>()->GetTexture("rangedEnemy");
	setTexure(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 50, 32));
	sprite.setScale(-3.f, 3.f);
	sprite.setOrigin(sprite.getLocalBounds().width / 1.5f, sprite.getLocalBounds().height / 2.f);
	healthbar.setSize(sprite.getGlobalBounds().width, 6.f);
	frame.setNumberOfFrames(3);
	frame.setDimension(50, 32);
	frame.setIdleSpeed(0.075f);
}

void RangedEnemy::updateAttack()
{
	for (int i = 0; i < attacks.size(); i++)
	{
		attacks[i].update();
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

void RangedEnemy::update()
{
	updateDirection();

	updateTimers();

	auto player = GetPlayerObject();
	auto playerpos = player->getPostion();

	updateAngle(playerpos);

	if (movingTimer >= ENEMY_RANGED_MOVING_TIMER)
	{
		movingTimer = -1.f;
		ac_state = EActionState::SHOOTING;
		state = EMovementState::IDLE;
	}
	if (ac_state == EActionState::SHOOTING)
	{
		createAttack(playerpos);
		updateAttack();
	}
	if (ac_state == EActionState::NOT_SHOOTING) {
		sprite.move(ENEMY_RANGED_SPEED * direction);
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
	if (ac_state != EActionState::DYING)
	{
		renderAttack(target);
		healthbar.render(target);
	}
}

bool RangedEnemy::attackHasHit(const sf::FloatRect& object)
{
	for (size_t i = 0; i < attacks.size(); i++)
	{
		if (attacks[i].getActionState() == EActionState::DYING && attacks[i].isFrameFinished()) {
			attacks.erase(attacks.begin() + i);
			return false;
		}
		if (attacks[i].getActionState() != EActionState::DYING && attacks[i].getGlobalBounds().intersects(object))
		{
			attacks[i].setActionState(EActionState::DYING);
			return true;
		}
	}
	return false;
}
