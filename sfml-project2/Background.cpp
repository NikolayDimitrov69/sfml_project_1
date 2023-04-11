#include "precompheaders.h"
#include "Background.h"

Background::Background()
{}

void Background::setTexture(const std::string& path)
{
	backgroundTexture.loadFromFile(path);
	backgroundSprite.setTexture(backgroundTexture);
}

void Background::setScale(const sf::Vector2f& scale)
{
	backgroundSprite.setScale(scale);
}

void Background::render(sf::RenderTarget& target)
{
	target.draw(backgroundSprite);
}
