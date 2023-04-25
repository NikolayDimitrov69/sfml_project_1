#include "precompheaders.h"
#include "Player.h"

void Player::initPlayerHealth()
{
	health.m_Health = 100.f;
	health.currentHealth = health.m_Health;
}

void Player::initSprite(const sf::Texture& texture, const sf::Vector2u& targetSize)
{
	
	frame.setDimension(28, 25);
	frame.setIdleSpeed(0.5f);
	frame.setTextureSize(texture.getSize());
	m_Sprite.setTexture(texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, 28, 25));
	m_Sprite.setScale(4.f, 4.f);
	m_Sprite.setOrigin(m_Sprite.getLocalBounds().width / 2, m_Sprite.getLocalBounds().height / 2);
	m_Sprite.setPosition(targetSize.x / 2.f, targetSize.y / 2.f);
}

void Player::initAttack()
{
	attack_texture.loadFromFile("IMAGES/megaman_attack.png");
}

Player::Player(const sf::Vector2u& targetSize, const sf::Texture& texture, std::string name, float newhealth) : m_Name(name)
{
	damage = PLAYER_DAMAGE;
	//Will be asumed that the player is not on a solid object, if needed the game class will change that
	physicstate = Physicstate::MID_AIR;
	//Player will be spawned with its idle animation
	playerstate = Movementstate::IDLE;
	health.m_Health = newhealth;
	initAttack();
	initSprite(texture, targetSize);
	initPlayerHealth();
}

const float& Player::dealDamage() const
{
	return damage;
}

bool Player::attackHasHit(const sf::FloatRect& enemyBounds)
{
	for (size_t i = 0; i < attacks.size(); i++)
	{
		if (attacks[i].getActionState() == Actionstate::DYING && attacks[i].isFrameFinished()) {
			attacks.erase(attacks.begin() + i);
			return false;
		}
		if (attacks[i].getActionState() != Actionstate::DYING && attacks[i].getGlobalBounds().intersects(enemyBounds))
		{
			attacks[i].setActionState(Actionstate::DYING);
			return true;
		}
	}
	return false;
}

const sf::Vector2f& Player::getPostion() const
{
	return sf::Vector2f(m_Sprite.getPosition());
}

void Player::takeDamage(float damage)
{
	if (health.currentHealth > 0.f)
		health.currentHealth -= damage;
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
	if (m_Sprite.getScale().x > 0)
		m_Sprite.setScale(-1.f * m_Sprite.getScale().x, m_Sprite.getScale().y);
}

void Player::TurnRight()
{
	if (m_Sprite.getScale().x < 0)
		m_Sprite.setScale(-1.f * m_Sprite.getScale().x, m_Sprite.getScale().y);
}

void Player::updateInputAndSates(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize)
{
	sf::FloatRect playerBounds = m_Sprite.getGlobalBounds();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		TurnLeft();
		move_x(-1.f);
		if (playerphysics.getMoveVelocity().y == 0)
			playerstate = Movementstate::MOVING;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		TurnRight();
		move_x(1.f);
		if (playerphysics.getMoveVelocity().y == 0)
			playerstate = Movementstate::MOVING;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && physicstate == Physicstate::ON_GROUND && keyPressable())
	{
		physicstate = Physicstate::MID_AIR;
		playerstate = Movementstate::JUMPING;
		jump(JUMP_FORCE);
		std::cout << "jump\n";	
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		actionstate = Actionstate::SHOOTING;
		if (mousePos.x < m_Sprite.getPosition().x)
			TurnLeft();
		if (mousePos.x > m_Sprite.getPosition().x)
			TurnRight();
		createDoubleAttack(mousePos, targetSize);
	}
	else
		actionstate = Actionstate::NOT_SHOOTING;

	if (playerphysics.getMoveVelocity().y == 0 && physicstate == Physicstate::MID_AIR)
		playerstate = Movementstate::FALLING;
	else if (playerphysics.getMoveVelocity() == sf::Vector2f(0.f, 0.f))
		playerstate = Movementstate::IDLE;
}

void Player::updateHealth()
{
	health.healthbar.update(m_Sprite, health.m_Health, health.currentHealth);
}

void Player::createSingleAttack(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize)
{
	if (attCooldown >= ATTACK_COOLDOWN && mousePos.x > 0 && mousePos.y > 0 && mousePos.x < targetSize.x && mousePos.y < targetSize.y)
	{
		attCooldown = 0.f;
		Attack attack(attack_texture);
		attack.setShootDir(mousePos, m_Sprite.getPosition());
		attack.changeDirection(mousePos.x < m_Sprite.getPosition().x ? -1 : 1);
		attack.spawn(m_Sprite);
		attacks.push_back(attack);
	}
}

void Player::createDoubleAttack(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize)
{
	if (attCooldown >= ATTACK_COOLDOWN && mousePos.x > 0 && mousePos.y > 0 && mousePos.x < targetSize.x && mousePos.y < targetSize.y)
	{
		attCooldown = 0.f;
		Attack attack(attack_texture);
		attack.changeDirection(mousePos.x < m_Sprite.getPosition().x ? -1 : 1);
		attack.setShootDir(mousePos, m_Sprite.getPosition(), 5);
		
		attack.spawn(m_Sprite);

		Attack attack2(attack_texture);
		attack2.changeDirection(mousePos.x < m_Sprite.getPosition().x ? -1 : 1);
		attack2.setShootDir(mousePos, m_Sprite.getPosition(), -5);
		attack2.spawn(m_Sprite);


		attacks.push_back(attack);

		attacks.push_back(attack2);
	}
}

void Player::updateAttack(const sf::Vector2u& targetSize)
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

void Player::updatePlayerPhysics()
{
	//Gravity will be stoped, if player is on ground
	if (physicstate == Physicstate::ON_GROUND)
		playerphysics.stopGravity();

	//Gravity will be applied only if the player is mid air, meaning no solid object bellow them
	if (physicstate == Physicstate::MID_AIR)
		applyGravity();

	playerphysics.updateMovePhysics();
	m_Sprite.move(playerphysics.getMoveVelocity());
}

void Player::applyGravity()
{
	playerphysics.updateGravity();
}

void Player::updateFrame()
{
	frame.update(playerstate, actionstate);
	m_Sprite.setTextureRect(frame.getCurrentFrame());
}

void Player::renderHealth(sf::RenderTarget& target)
{
	health.healthbar.render(target);
}

void Player::renderSprite(sf::RenderTarget& target)
{
	target.draw(m_Sprite);
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
	return m_Sprite.getGlobalBounds();
}

void Player::updatePlayer(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize)
{
	timer += 1.f;
	attCooldown += 1.f;
	updateHealth();
	updateAttack(targetSize);
	updatePlayerPhysics();
	updateInputAndSates(mousePos, targetSize);
	updateFrame();
}

void Player::setPhysicState(const Physicstate& newstate)
{
	physicstate = newstate;
}

const Physicstate& Player::getPhysState() const
{
	return physicstate;
}

void Player::renderPlayer(sf::RenderTarget& target)
{
	renderHealth(target);
	renderSprite(target);
	renderAttack(target);
}

bool Player::keyPressable()
{
	if (timer >= JUMP_COOLDOWN) {
		timer = 0.f;
		return true;
	}
	return false;
}



