#include "Attack.h"

Attack::Attack() : m_Direction(1), outOfBounds(false)
{}

void Attack::changeDirection(const int& direction)
{
	m_Direction = direction;
}

void Attack::update(sf::RenderTarget& target)
{
	m_Sprite.move(m_Direction * 10.f, 0);
	if (m_Sprite.getPosition().x > target.getSize().x || m_Sprite.getPosition().x < 0)
		outOfBounds = true;
}

bool Attack::isOutOfBounds() const
{
	return outOfBounds;
}

void Attack::spawn(const sf::Sprite& sprite)
{
	m_Sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - m_Sprite.getLocalBounds().height / 2);
	m_Sprite.setScale(m_Direction, 1);
}

void Attack::setTexture(const std::string& texture_Path)
{
	m_Texture.loadFromFile(texture_Path);
	m_Sprite.setTexture(m_Texture);
}

void Attack::render(sf::RenderTarget& target) {
	target.draw(m_Sprite);
}
