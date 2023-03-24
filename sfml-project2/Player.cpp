#include "Player.h"

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
	maxAttacks = 2;
	attack.setTexture("IMAGES/megaman_attack.png");
}

Player::Player(sf::RenderTarget& target, std::string name, int health, std::string texture) :m_Target(target), m_Name(name), m_Health(health)
{
	moveSpeed = 3.f;
	initAttack();
	initSprite(texture);
	initPlayerInfo();
}

const sf::Vector2f& Player::getPostion() const
{
	return sf::Vector2f(m_Sprite.getPosition());
}

void Player::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (m_Sprite.getScale().x > 0)
			m_Sprite.setScale(-1.f * m_Sprite.getScale().x, m_Sprite.getScale().y);
		m_Sprite.move(-moveSpeed, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_Sprite.getScale().x < 0)
			m_Sprite.setScale(-1.f * m_Sprite.getScale().x, m_Sprite.getScale().y);
		m_Sprite.move(moveSpeed, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_Sprite.move(0.f, -moveSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_Sprite.move(0.f, moveSpeed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		fillAttackVector();
	}
}

void Player::updatePlayerInfo()
{
	std::stringstream ss;
	ss << "HP: " << m_Health << '\n' << m_Name;
	PlayerInfo.setString(ss.str());
	PlayerInfo.setPosition(m_Sprite.getPosition().x - 50, m_Sprite.getPosition().y - 110);
}

void Player::fillAttackVector()
{
	if (maxAttacks >= 0)
	{
		attack.changeDirection(m_Sprite.getScale().x < 0 ? -1 : 1);
		attack.spawn(m_Sprite);

		maxAttacks--;
		attacks.push_back(attack);
	}
}

void Player::updateAttack()
{
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
	m_Target.draw(PlayerInfo);
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

void Player::renderPlayer()
{
	renderPlayerInfo();
	renderSprite();
	renderAttack();
}

void Player::updatePlayer()
{
	updatePlayerInfo();
	updateAttack();
	updateInput();
}

