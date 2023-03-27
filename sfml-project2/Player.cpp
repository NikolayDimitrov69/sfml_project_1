#include "Player.h"

void Player::initPlayerHealth()
{
	m_Health = 100;
	currentHealth = m_Health;
}

void Player::initPlayerInfo()
{
	font.loadFromFile("Fonts/arial.ttf");
	PlayerInfo.setCharacterSize(20);
	PlayerInfo.setFillColor(sf::Color::White);
	PlayerInfo.setFont(font);
}

void Player::initSprite(std::string& texture)
{
	m_Texture.loadFromFile(texture);
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setScale(0.05, 0.05);
	m_Sprite.setOrigin(m_Sprite.getLocalBounds().width / 2, m_Sprite.getLocalBounds().height / 2);
	m_Sprite.setPosition(m_Target.getSize().x / 2.f, m_Target.getSize().y / 2.f);
}

void Player::initAttack()
{
	maxAttCooldown = 30;
	attCooldown = maxAttCooldown;
	maxAttacks = 2;
	attack.setTexture("IMAGES/megaman_attack.png");
}

Player::Player(sf::RenderTarget& target, std::string name, int health, std::string texture) :m_Target(target), m_Name(name), m_Health(health)
{
	moveSpeed = 3.f;
	initAttack();
	initSprite(texture);
	initPlayerHealth();
	initPlayerInfo();
}

const sf::Vector2f& Player::getPostion() const
{
	return sf::Vector2f(m_Sprite.getPosition());
}

float Player::getMaxHealth() const
{
	return m_Health;
}

float Player::getCurrentHealth() const
{
	return currentHealth;
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

void Player::updateInput()
{
	sf::FloatRect playerBounds = m_Sprite.getGlobalBounds();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		TurnLeft();
		if (playerBounds.left - moveSpeed > 0.f)
			m_Sprite.move(-moveSpeed, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		TurnRight();
		if (playerBounds.left + playerBounds.width + moveSpeed < m_Target.getSize().x)
			m_Sprite.move(moveSpeed, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (playerBounds.top - moveSpeed > 0.f)
			m_Sprite.move(0.f, -moveSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (playerBounds.top + playerBounds.height + moveSpeed < m_Target.getSize().y)
			m_Sprite.move(0.f, moveSpeed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		fillAttackVector();
	}

	//Test button - testing the healthbar class
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		currentHealth -= 0.5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		currentHealth += 0.5;
	//Dev move ^^^ >:)
}

void Player::updatePlayerInfo()
{
	healthbar.update(m_Sprite, m_Health, currentHealth);
}

void Player::fillAttackVector()
{
	if (maxAttacks >= 0 && maxAttCooldown == attCooldown)
	{
		attCooldown = 0;
		attack.changeDirection(m_Sprite.getScale().x < 0 ? -1 : 1);
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
		attacks[i].update(m_Target);
		if (attacks[i].isOutOfBounds())
		{
			attacks.erase(attacks.begin() + i);
			maxAttacks++;
		}
	}
}

void Player::renderPlayerInfo()
{
	//m_Target.draw(PlayerInfo);
	healthbar.render(m_Target);
}

void Player::renderSprite()
{
	m_Target.draw(m_Sprite);
}

void Player::renderAttack()
{
	for (auto& e : attacks)
	{
		e.render(m_Target);
	}
}

void Player::updatePlayer()
{
	updatePlayerInfo();
	updateAttack();
	updateInput();
}

void Player::renderPlayer()
{
	renderPlayerInfo();
	renderSprite();
	renderAttack();
}



