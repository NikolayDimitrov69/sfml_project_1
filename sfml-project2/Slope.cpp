#include "precompheaders.h"
#include "Slope.h"

Slope::Slope(const sf::Texture& texture)
{
	slopeSprite.setTexture(texture);
}

void Slope::setPostion(const sf::Vector2f& pos)
{
	slopeSprite.setPosition(pos);
}

sf::FloatRect Slope::getGlobalBounds() const
{
	return slopeSprite.getGlobalBounds();
}

void Slope::setRandomVertPos(const sf::Vector2u& targetSize, sf::Vector2f& prev_pos)
{
	float min = -200.f;
	float max = 200.f;
	float randx{};

	float farRight = (3.f * targetSize.x) / 4.f - slopeSprite.getGlobalBounds().width;
	float farLeft = targetSize.x / 4.f;

	if (prev_pos.x == farRight)
	{
		min *= 2;
		max = 1;
	}
	if (prev_pos.x == farLeft)
	{
		min = -1;
		max *= 2;
	}

	randx = min + rand() % static_cast<int>(max - min + 1);

	if (prev_pos.x == -1)
		prev_pos.x = static_cast<float>(farLeft + rand() % static_cast<int>(farRight - farLeft + 1 - slopeSprite.getGlobalBounds().width));

	float finalpos = prev_pos.x + randx;
	if (finalpos >= farRight)
		finalpos = farRight;
	if (finalpos < farLeft)
		finalpos = farLeft;

	slopeSprite.setPosition(finalpos, 0.f);

	prev_pos = slopeSprite.getPosition();
}

float Slope::getFallSpeed() const
{
	return fallSpeed;
}

void Slope::setFallSpeed(const float& newSpeed)
{
	fallSpeed = newSpeed;
}

void Slope::setScale(const sf::Vector2f& size)
{
	slopeSprite.setScale(size);
}

const sf::Vector2f& Slope::getPosition() const
{
	return slopeSprite.getPosition();
}

void Slope::update()
{
	slopeSprite.move(0.f, fallSpeed);
}

void Slope::render(sf::RenderTarget& target)
{
	target.draw(slopeSprite);
}
