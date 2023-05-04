#include "precompheaders.h"
#include "Bar.h"

void Bar::initColor()
{
	bar.setFillColor(sf::Color::Green);
}

void Bar::initOutline()
{
	outline.setOutlineColor(sf::Color::Black);
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(4.f);
}

Bar::Bar() : Percentage(1)
{
	outline.setPosition(-10.f, -10.f);
	initOutline();
	initColor();
}

void Bar::setSize(const float& nwidth, const float& nheight)
{
	outline.setSize(sf::Vector2f(nwidth, nheight));
}

void Bar::updatePercentage(const float& maxValue, const float& currentValue)
{
	if (currentValue < 0)
		Percentage = 0.f;
	else if (currentValue >= 0 && currentValue <= maxValue)
		Percentage = currentValue / maxValue;
}

void Bar::updateSize()
{
	float fullSize = outline.getGlobalBounds().width - (2.f * outline.getOutlineThickness());
	bar.setSize(sf::Vector2f(fullSize * Percentage, outline.getGlobalBounds().height - 8.f));
}

void Bar::updatePosition(const sf::Sprite& sprite)
{
	outline.setPosition(sprite.getGlobalBounds().left,
		sprite.getGlobalBounds().top - 15.f);
	bar.setPosition(outline.getGlobalBounds().left + 4.f, outline.getGlobalBounds().top + 4.f);
}

void Bar::updateColor()
{
	if (Percentage <= 0.3)
		bar.setFillColor(sf::Color::Red);
	else
		bar.setFillColor(sf::Color::Green);
}

void Bar::update(const sf::Sprite& sprite, const float& maxValue, const float& currentValue)
{
	updatePercentage(maxValue, currentValue);
	updateSize();
	updateColor();
	updatePosition(sprite);
}

void Bar::setPosition(const sf::Vector2f& pos)
{
	outline.setPosition(pos);
	bar.setPosition(outline.getPosition().x, outline.getPosition().y);
}

void Bar::updateStatic(const float& maxValue, const float& currentValue)
{
	updatePercentage(maxValue, currentValue);
	updateSize();
}

void Bar::render(sf::RenderTarget& target)
{
	target.draw(outline);
	target.draw(bar);
}
