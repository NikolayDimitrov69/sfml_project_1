#include "precompheaders.h"
#include "Player.h"

void Player::initPlayerHealth()
{
	health.m_Health = 100;
	health.currentHealth = health.m_Health;
}

void Player::initSprite(std::string& texture)
{
	m_Texture.loadFromFile(texture);
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setScale(0.05f, 0.05f);
	m_Sprite.setOrigin(m_Sprite.getLocalBounds().width / 2, m_Sprite.getLocalBounds().height / 2);
	m_Sprite.setPosition(m_Target->getSize().x / 2.f, m_Target->getSize().y / 2.f);
}

void Player::initAttack()
{
	maxAttCooldown = 30;
	attCooldown = maxAttCooldown;
	maxAttacks = 2;
	attack.setTexture("IMAGES/megaman_attack.png");
}

Player::Player(sf::RenderTarget* target, std::string name, float newhealth, std::string texture) :m_Target(target), m_Name(name)
{
	//Will be asumed that the player is not on a solid object, if needed the game class will change that
	physicstate = Physicstate::MID_AIR;
	//Player will be spawned with its idle animation
	playerstate = Playerstate::IDLE;
	health.m_Health = newhealth;
	initAttack();
	initSprite(texture);
	initPlayerHealth();
}

const sf::Vector2f& Player::getPostion() const
{
	return sf::Vector2f(m_Sprite.getPosition());
}

float Player::getMaxHealth() const
{
	return health.m_Health;
}

float Player::getCurrentHealth() const
{
	return health.currentHealth;
}

float Player::getBottomHitbox() const
{
	return m_Sprite.getGlobalBounds().top + m_Sprite.getGlobalBounds().height;
}

void Player::setPhysicState(const Physicstate& newstate)
{
	physicstate = newstate;
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

void Player::updateInput(const sf::Vector2f& pos)
{
	sf::FloatRect playerBounds = m_Sprite.getGlobalBounds();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		TurnLeft();
		playerstate = Playerstate::MOVE_LEFT;
		//if (playerBounds.left - moveSpeed > 0.f)
		move_x(-1.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		TurnRight();
		playerstate = Playerstate::MOVE_RIGHT;
		//if (playerBounds.left + playerBounds.width + moveSpeed < m_Target->getSize().x)
		move_x(1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && physicstate == Physicstate::ON_GROUND)
	{
		physicstate = Physicstate::MID_AIR;
		playerstate = Playerstate::JUMPING;
		jump(7.f);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (pos.x < m_Sprite.getPosition().x)
			TurnLeft();
		if (pos.x > m_Sprite.getPosition().x)
			TurnRight();
		fillAttackVector(pos);
	}

	//Test button - testing the healthbar class
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		health.currentHealth -= 0.5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		health.currentHealth += 0.5;
	//Dev move ^^^ >:)
}

Player::Player() : m_Target(nullptr)
{}

void Player::updatePlayerInfo()
{
	health.healthbar.update(m_Sprite, health.m_Health, health.currentHealth);
}

void Player::fillAttackVector(const sf::Vector2f& pos)
{
	if (maxAttacks >= 0 && maxAttCooldown == attCooldown && pos.x > 0 && pos.y > 0 && pos.x < m_Target->getSize().x && pos.y < m_Target->getSize().y)
	{
		attCooldown = 0;
		attack.setShootDir(pos, m_Sprite.getPosition());
		attack.changeDirection(pos.x < m_Sprite.getPosition().x ? -1 : 1);
		attack.spawn(m_Sprite);

		maxAttacks--;
		attacks.push_back(attack);
	}
}

void Player::updateAttack()
{
	if (attCooldown < maxAttCooldown)
		attCooldown += 1;
	for (int i = 0; i < attacks.size(); i++)
	{
		attacks[i].update(*m_Target);
		if (attacks[i].isOutOfBounds())
		{
			attacks.erase(attacks.begin() + i);
			maxAttacks++;
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

void Player::renderPlayerInfo()
{
	health.healthbar.render(*m_Target);
}

void Player::renderSprite()
{
	m_Target->draw(m_Sprite);
}

void Player::renderAttack()
{
	for (auto& e : attacks)
	{
		e.render(*m_Target);
	}
}

void Player::updatePlayer(const sf::Vector2f& pos)
{
	updatePlayerInfo();
	updateAttack();
	updatePlayerPhysics();
	updateInput(pos);
	
}

void Player::renderPlayer()
{
	renderPlayerInfo();
	renderSprite();
	renderAttack();
}



