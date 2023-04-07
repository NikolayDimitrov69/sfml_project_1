#include "precompheaders.h"
#include "Healthbar.h"

void Healthbar::initHealth()
{
	health.setFillColor(sf::Color::Green);
}

void Healthbar::initOutline()
{
	outline.setOutlineColor(sf::Color::Black);
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(4.f);
}

Healthbar::Healthbar() : Percentage(1)
{
	initOutline();
	initHealth();
}

void Healthbar::updatePercentage(const float& maxhp, const float& currenthp)
{
	if (currenthp < 0)
		Percentage = 0.f;
	else if (currenthp >= 0 && currenthp <= maxhp)
		Percentage = currenthp / maxhp;
}

void Healthbar::updateSize(const sf::Sprite& sprite)
{
	outline.setSize(sf::Vector2f(sprite.getGlobalBounds().width, 6.f));
	float fullHp = outline.getGlobalBounds().width - (2.f * outline.getOutlineThickness());
	health.setSize(sf::Vector2f(fullHp * Percentage, 6.f));
}

void Healthbar::updatePosition(const sf::Sprite& sprite)
{
	outline.setPosition(sprite.getGlobalBounds().left,
		sprite.getGlobalBounds().top - 15.f);
	health.setPosition(outline.getGlobalBounds().left + 4.f, outline.getGlobalBounds().top + 4.f);
}

void Healthbar::updateColor()
{
	if (Percentage <= 0.3)
		health.setFillColor(sf::Color::Red);
	else
		health.setFillColor(sf::Color::Green);
}

void Healthbar::update(const sf::Sprite& sprite, const float& maxhp, const float& currenthp)
{
	updatePercentage(maxhp, currenthp);
	updateColor();
	updateSize(sprite);
	updatePosition(sprite);
}

void Healthbar::render(sf::RenderTarget& target)
{
	target.draw(outline);
	target.draw(health);
}
