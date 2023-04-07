#include "precompheaders.h"
#include "Slope.h"

Slope::Slope()
{
	prev_pos.x = -1;
	fallSpeed = INIT_SLOPE_FALL_SPEED;
	slopeTexture.loadFromFile("IMAGES/platform.jpg");
	slopeSprite.setTexture(slopeTexture);
	slopeSprite.setScale(3.f, 3.f);
}

const sf::FloatRect& Slope::getGlobalBounds() const
{
	return slopeSprite.getGlobalBounds();
}

void Slope::setRandomVertPos(sf::RenderTarget& target)
{
	float min = -200.f;
	float max = 200.f;
	float randx{};

	float farRight = (3.f * target.getSize().x) / 4.f - slopeSprite.getGlobalBounds().width;
	float farLeft = target.getSize().x / 4.f;

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
		prev_pos.x = static_cast<float>(target.getSize().x + rand() % static_cast<int>(target.getSize().x/2.f - slopeSprite.getGlobalBounds().width));

	float finalpos = prev_pos.x + randx;
	if (finalpos >= farRight)
		finalpos = farRight;
	if (finalpos < farLeft)
		finalpos = farLeft;

	slopeSprite.setPosition(finalpos, 0.f);

	prev_pos = slopeSprite.getPosition();
}

void Slope::setFallSpeed(const float& newSpeed)
{
	fallSpeed = newSpeed;
}

void Slope::update()
{
	slopeSprite.move(0.f, fallSpeed);
}

void Slope::render(sf::RenderTarget& target)
{
	target.draw(slopeSprite);
}
