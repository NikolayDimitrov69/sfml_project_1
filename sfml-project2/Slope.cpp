#include "precompheaders.h"
#include "Slope.h"

Slope::Slope()
{
	prev_pos.x = -1;
	fallSpeed = 1.2f;
	slopeTexture.loadFromFile("IMAGES/platform.jpg");
	slopeSprite.setTexture(slopeTexture);
	slopeSprite.setScale(3.f, 3.f);
}

const float& Slope::getSlopeTop() const
{
	return slopeSprite.getGlobalBounds().top;
}

const float& Slope::getSlopeLeft() const
{
	return slopeSprite.getGlobalBounds().left;
}

const float& Slope::getSlopeRight() const
{
	return slopeSprite.getGlobalBounds().left + slopeSprite.getGlobalBounds().width;
}

sf::FloatRect Slope::getGlobalBounds() const
{
	return slopeSprite.getGlobalBounds();
}

void Slope::setRandomVertPos(sf::RenderTarget& target)
{
	float min = -300.f;
	float max = 300.f;
	float randx{};
	randx = min + rand() % static_cast<int>(max - min + 1);

	if (prev_pos.x == -1)
		prev_pos.x = static_cast<float>(rand() % static_cast<int>(target.getSize().x - slopeSprite.getGlobalBounds().width));

	float finalpos = prev_pos.x + randx;
	if (finalpos >= target.getSize().x - slopeSprite.getGlobalBounds().width)
		finalpos = target.getSize().x - slopeSprite.getGlobalBounds().width;
	if (finalpos < 0)
		finalpos = 0;

	slopeSprite.setPosition(finalpos,
		0.f);
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
