#include "precompheaders.h"
#include "Attack.h"
#include "Constants.h"

void Attack::updateFrame()
{
	frame.update(state, ac_state);
	m_Sprite.setTextureRect(frame.getCurrentFrame());
}

void Attack::initVariables() {
	shootDir.x = -1;
	shootDir.y = -1;
	state = EMovementState::IDLE;
	ac_state = EActionState::NOT_SHOOTING;
}

Attack::Attack(const sf::Texture& attack_texture, const int& attackWidth, const int& attackHeigth)
	: Attack(attack_texture, attackWidth, attackHeigth, PLAYER_DEFAULT_ATTACK_MOVE_SPEED, 1.0f)
{
}

Attack::Attack(const sf::Texture& attack_texture, const int& attackWidth, const int& attackHeigth, const float& nattackMoveSpeed, const float& scale)
	: m_Direction(1)
	, outOfBounds(false)
	, angle(0.f)
	, attackMoveSpeed(nattackMoveSpeed)
	, attackScale(scale)
	, shootDir(-1, -1)
	, state(EMovementState::IDLE)
	, ac_state(EActionState::NOT_SHOOTING)
{
	m_Sprite.setTexture(attack_texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, attackWidth, attackHeigth));
	frame.setTextureSize(attack_texture.getSize());
	frame.setDimension(attackWidth, attackHeigth);
	frame.setIdleSpeed(0.03f);
	frame.setNumberOfFrames(3);
}

bool Attack::isFrameFinished() const
{
	return frame.isFinished();
}

void Attack::setActionState(const EActionState& nstate)
{
	ac_state = nstate;
}

sf::FloatRect Attack::getGlobalBounds() const {
	return m_Sprite.getGlobalBounds();
}

void Attack::setShootDir(const sf::Vector2f& mousepos, const sf::Vector2f& playerpos, const float& offsetAngle)
{
	sf::Vector2f unitVectorPlayerMouse = normalize(mousepos - playerpos);
	angle = findAngleTan(playerpos, mousepos);
	//if offset is 0, then shootDir will simply be the vector between the player and the mouse position
	if (offsetAngle == 0)
	{
		shootDir = unitVectorPlayerMouse;
	}
	else //the vector will be created by the following formula
	{
		float r = vectorLenght(mousepos - playerpos);
		shootDir.x = r * cos((angle + offsetAngle) * UTILITY_M_PI / 180.f);
		shootDir.y = r * sin((angle + offsetAngle) * UTILITY_M_PI / 180.f);

		shootDir = normalize(shootDir) * m_Direction;

		angle += offsetAngle;
	}
	m_Sprite.setRotation(angle);
}

void Attack::changeDirection(const float& direction)
{
	m_Direction = direction;
}

void Attack::update(const sf::Vector2u& targetSize)
{
	if (ac_state == EActionState::NOT_SHOOTING)
	{
		m_Sprite.move(attackMoveSpeed * shootDir);
		if (m_Sprite.getPosition().x > targetSize.x ||
			m_Sprite.getPosition().x < 0 ||
			m_Sprite.getPosition().y < 0 ||
			m_Sprite.getPosition().y > targetSize.y)
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
	m_Sprite.setScale(m_Direction * attackScale, 1 * attackScale);
}

const EActionState& Attack::getActionState() const
{
	return ac_state;
}

void Attack::render(sf::RenderTarget& target) {
	target.draw(m_Sprite);
}
