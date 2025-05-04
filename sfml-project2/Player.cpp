#include "precompheaders.h"
#include "Player.h"
#include "PlayerManagerService.h"

void Player::initPlayerHealth()
{
	health.healthbar.setSize(GetService<PlayerManagerService>()->GetSprite().getGlobalBounds().width, 6.f);
}

void Player::initSprite()
{
	const auto& sprite = GetService<PlayerManagerService>()->GetSprite();
	frame.setDimension(28, 25);
	frame.setIdleSpeed(0.5f);
	frame.setTextureSize(sprite.getTexture()->getSize());
	frame.setNumberOfFrames(3);
}

void Player::initAttack()
{
	attack_texture.loadFromFile("IMAGES/megaman_attack.png");
}

Player::Player()
	: m_Name("player")
	, health(PLAYER_MAX_HEALTH)
	, boostAttackTimer(PLAYER_BOOST_ATTACK_TIMER)
	, attackCooldown(PLAYER_ATTACK_COOLDOWN)
	, doubleJumpTimer(0.f)
	, doubleJumpedOnce(false)
	, doubleJumps(0)
	, attCooldown(PLAYER_ATTACK_COOLDOWN)
	, doubleAttCooldown(PLAYER_DOUBLE_ATTACK_TIMER)
	, damage(PLAYER_DAMAGE)
	, physicstate(EPhysicState::MID_AIR)
	, playerstate(EMovementState::IDLE)
	, timer(0.f)
	, actionstate(EActionState::INVALID)
{
}

void Player::heal(float amount)
{
	health.currentHealth += amount;
	if (health.currentHealth >= health.m_Health)
	{
		health.currentHealth = health.m_Health;
	}
}

const float& Player::dealDamage() const
{
	return damage;
}

void Player::parentTo(float speed)
{
	playerphysics.setVelocity_Y(speed);
}

bool Player::attackHasHit(const sf::FloatRect& enemyBounds)
{
	for (size_t i = 0; i < attacks.size(); i++)
	{
		if (attacks[i].getActionState() == EActionState::DYING && attacks[i].isFrameFinished()) {
			attacks.erase(attacks.begin() + i);
			return false;
		}
		if (attacks[i].getActionState() != EActionState::DYING && attacks[i].getGlobalBounds().intersects(enemyBounds))
		{
			attacks[i].setActionState(EActionState::DYING);
			return true;
		}
	}
	return false;
}

sf::Vector2f Player::getPostion() const
{
	return sf::Vector2f(GetService<PlayerManagerService>()->GetSprite().getPosition());
}

void Player::takeDamage(float damage)
{
	if (health.currentHealth > 0.f)
		health.currentHealth -= damage;
}

int Player::getDoubleJumps() const
{
	return doubleJumps;
}

void Player::gainDoubleJump()
{
	if (doubleJumps <= PLAYER_MAX_DOUBLE_JUMPS)
		doubleJumps += 1;
}

const float& Player::getMaxHealth() const
{
	return health.m_Health;
}

const float& Player::getCurrentHealth() const
{
	return health.currentHealth;
}

void Player::move_x(const float& dir_x)
{
	playerphysics.setVelocity_X(dir_x);
}

void Player::jump(const float& height)
{
	playerphysics.setVelocity_Y(height);
}

void Player::TurnLeft()
{
	auto& sprite = GetService<PlayerManagerService>()->MutableSprite();
	if (sprite.getScale().x > 0)
		sprite.setScale(-1.f * sprite.getScale().x, sprite.getScale().y);
}

void Player::TurnRight()
{
	auto& sprite = GetService<PlayerManagerService>()->MutableSprite();
	if (sprite.getScale().x < 0)
		sprite.setScale(-1.f * sprite.getScale().x, sprite.getScale().y);
}

void Player::updateAttackCooldown()
{
	if (boostAttackTimer < PLAYER_BOOST_ATTACK_TIMER)
	{
		attackCooldown = PLAYER_ATTACK_COOLDOWN / 2;
	}
	else
	{
		attackCooldown = PLAYER_ATTACK_COOLDOWN;
	}

}

void Player::updateInputAndSates(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize)
{
	 const auto& sprite = GetService<PlayerManagerService>()->GetSprite();
	
	sf::FloatRect playerBounds = sprite.getGlobalBounds();

	if (physicstate == EPhysicState::ON_GROUND)
	{
		doubleJumpedOnce = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		TurnLeft();
		move_x(-1.f);
		if (playerphysics.getMoveVelocity().y == 0)
			playerstate = EMovementState::MOVING;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		TurnRight();
		move_x(1.f);
		if (playerphysics.getMoveVelocity().y == 0)
			playerstate = EMovementState::MOVING;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && physicstate == EPhysicState::ON_GROUND && keyPressable())
	{
		physicstate = EPhysicState::MID_AIR;
		playerstate = EMovementState::JUMPING;
		jump(PLAYER_JUMP_FORCE);
	}

	if (!doubleJumpedOnce && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && physicstate == EPhysicState::MID_AIR && doubleJumpTimer >= PLAYER_MIN_DOUBLE_JUMP_TIMER && doubleJumps > 0)
	{
		doubleJumpedOnce = true;
		jump(PLAYER_JUMP_FORCE);
		doubleJumpTimer = 0.f;
		doubleJumps--;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		actionstate = EActionState::SHOOTING;
		if (mousePos.x < sprite.getPosition().x)
			TurnLeft();
		if (mousePos.x > sprite.getPosition().x)
			TurnRight();

		if (doubleAttCooldown < PLAYER_DOUBLE_ATTACK_TIMER)
			createAttack(mousePos, targetSize, GAME_DEFAULT_ATTACK_COUNT * 2);

		createAttack(mousePos, targetSize, GAME_DEFAULT_ATTACK_COUNT);
	}
	else
		actionstate = EActionState::NOT_SHOOTING;

	if (playerphysics.getMoveVelocity().y == 0 && physicstate == EPhysicState::MID_AIR)
		playerstate = EMovementState::FALLING;
	else if (playerphysics.getMoveVelocity() == sf::Vector2f(0.f, 0.f))
		playerstate = EMovementState::IDLE;
}

void Player::updateHealth()
{
	health.healthbar.update(GetService<PlayerManagerService>()->GetSprite(), health.m_Health, health.currentHealth);
}

void Player::createAttack(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize, int amount)
{
	const auto& sprite = GetService<PlayerManagerService>()->GetSprite();
	if (attCooldown >= attackCooldown && mousePos.x > 0 && mousePos.y > 0 && mousePos.x < targetSize.x && mousePos.y < targetSize.y)
	{
		attCooldown = 0.f;
		float baseAngle = -(GAME_ATTACK_OFFSET_ANGLE * (amount - 1) / 2.f);
		for (int i = 0; i < amount; i++)
		{
			float angleOffset = baseAngle + i * GAME_ATTACK_OFFSET_ANGLE;
			Attack attack(attack_texture, 194, 60);
			attack.changeDirection(mousePos.x < sprite.getPosition().x ? -1 : 1);
			attack.setShootDir(mousePos, sprite.getPosition(), angleOffset);
			attack.spawn(sprite);
			attacks.push_back(attack);
		}
	}
}

void Player::updateAttack(const sf::Vector2u& targetSize)
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

void Player::updatePlayerPhysics()
{
	//Gravity will be stoped, if player is on ground
	if (physicstate == EPhysicState::ON_GROUND)
		playerphysics.stopGravity();

	//Gravity will be applied only if the player is mid air, meaning no solid object bellow them
	if (physicstate == EPhysicState::MID_AIR)
		applyGravity();

	playerphysics.updateMovePhysics();
	GetService<PlayerManagerService>()->MutableSprite().move(playerphysics.getMoveVelocity());
}

void Player::applyGravity()
{
	playerphysics.updateGravity();
}

void Player::updateFrame()
{
	frame.update(playerstate, actionstate);
	GetService<PlayerManagerService>()->MutableSprite().setTextureRect(frame.getCurrentFrame());
}

void Player::renderHealth(sf::RenderTarget& target)
{
	health.healthbar.render(target);
}

void Player::renderSprite(sf::RenderTarget& target)
{
	target.draw(GetService<PlayerManagerService>()->GetSprite());
}

void Player::renderAttack(sf::RenderTarget& target)
{
	for (auto& e : attacks)
	{
		e.render(target);
	}
}

sf::FloatRect Player::getGlobalBounds() const
{
	return GetService<PlayerManagerService>()->GetSprite().getGlobalBounds();
}

void Player::updateTimers()
{
	timer += 1.f;
	attCooldown += 1.f;
	doubleAttCooldown += 1.f;
	boostAttackTimer += 1.f;

	if (physicstate == EPhysicState::MID_AIR)
	{
		doubleJumpTimer += 1.f;
	}
	if (physicstate == EPhysicState::ON_GROUND)
	{
		doubleJumpTimer = 0.f;
	}
}

void Player::resetBoostTimer()
{
	boostAttackTimer = 0.f;
}

void Player::updatePlayer(const sf::Vector2f& mousePos)
{
	auto window = GetGameWindow();
	auto targetSize = window->getSize();
	updateAttackCooldown();
	updateTimers();
	updateHealth();
	updateAttack(targetSize);
	updatePlayerPhysics();
	updateInputAndSates(mousePos, targetSize);
	updateFrame();
}

void Player::setPhysicState(const EPhysicState& newstate)
{
	physicstate = newstate;
}

const EPhysicState& Player::getPhysState() const
{
	return physicstate;
}

void Player::renderPlayer(sf::RenderTarget& target)
{
	renderSprite(target);
	renderAttack(target);
	renderHealth(target);
}

bool Player::keyPressable()
{
	if (timer >= PLAYER_JUMP_COOLDOWN) {
		timer = 0.f;
		return true;
	}
	return false;
}

void Player::resetDoubleAttTimer()
{
	doubleAttCooldown = 0.f;
}

float Player::getDoubleAttackTimer() const
{
	return doubleAttCooldown;
}

float Player::getBoostAttackTimer() const
{
	return boostAttackTimer;
}

void Player::initialize()
{
	initAttack();
	initSprite();
	initPlayerHealth();
}


