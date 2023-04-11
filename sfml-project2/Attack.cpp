#include "precompheaders.h"
#include "Attack.h"

void Attack::updateFrame()
{
	frame.update(state, ac_state);
	m_Sprite.setTextureRect(frame.getCurrentFrame());
}

Attack::Attack() : m_Direction(1), outOfBounds(false), angle(0.f)
{
	shootDir.x = -1;
	shootDir.y = -1;
	state = Movementstate::IDLE;
	ac_state = Actionstate::NOT_SHOOTING;
	frame.setDimension(194, 60);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, 194, 60));
	frame.setIdleSpeed(0.03f);
}

bool Attack::isFrameFinished() const
{
	return frame.isFinished();
}

void Attack::setActionState(const Actionstate& nstate)
{
	ac_state = nstate;
}

const sf::FloatRect& Attack::getGlobalBounds() const {
	return m_Sprite.getGlobalBounds();
}

void Attack::setShootDir(const sf::Vector2f& mousepos, const sf::Vector2f& playerpos)
{
	shootDir = normalize(mousepos - playerpos);
	angle = findAngleTan(playerpos, mousepos);
}

void Attack::changeDirection(const int& direction)
{
	m_Direction = direction;
	m_Sprite.setRotation(angle);
}


void Attack::update(sf::RenderTarget& target)
{
	if (ac_state == Actionstate::NOT_SHOOTING)
	{
		m_Sprite.move(ATTACK_MOVE_SPEED * shootDir);
		if (m_Sprite.getPosition().x > target.getSize().x ||
			m_Sprite.getPosition().x < 0 ||
			m_Sprite.getPosition().y < 0 ||
			m_Sprite.getPosition().y > target.getSize().y)
			outOfBounds = true;
	}
	updateFrame();
}

bool Attack::isOutOfBounds() const
{
	return outOfBounds;
}

void Attack::spawn(const sf::Sprite& sprite)
{
	m_Sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - m_Sprite.getLocalBounds().height / 2.f);
	m_Sprite.setScale(m_Direction, 1);
}

void Attack::setTexture(const std::string& texture_Path)
{
	m_Texture.loadFromFile(texture_Path);
	m_Sprite.setTexture(m_Texture);
	frame.setTextureSize(m_Texture.getSize());
}

const Actionstate& Attack::getActionState() const
{
	return ac_state;
}

void Attack::render(sf::RenderTarget& target) {
	target.draw(m_Sprite);
}
