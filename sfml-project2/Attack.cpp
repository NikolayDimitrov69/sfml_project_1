#include "Attack.h"

Attack::Attack() : m_Direction(1), outOfBounds(false), angle(0.f)
{
	shootDir.x = -1;
	shootDir.y = -1;
}

void Attack::setShootDir(const sf::Vector2f& mousepos, const sf::Vector2f& playerpos)
{
	shootDir = mousepos - playerpos;
	shootDir = shootDir / static_cast<float>(sqrt(pow(shootDir.x, 2) + pow(shootDir.y, 2)));



	float julto = playerpos.y - mousepos.y;
	float cherveno = playerpos.x - mousepos.x;
	float tangens = julto / cherveno;
	angle = static_cast<float>(atan(tangens));
	angle = (angle * 180) / 3.1415;
}

void Attack::changeDirection(const int& direction)
{
	m_Direction = direction;
	m_Sprite.setRotation(angle);
}


void Attack::update(sf::RenderTarget& target)
{
	m_Sprite.move(5.f * shootDir);
	if (m_Sprite.getPosition().x > target.getSize().x || 
		m_Sprite.getPosition().x < 0 || 
		m_Sprite.getPosition().y < 0 || 
		m_Sprite.getPosition().y > target.getSize().y)
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
