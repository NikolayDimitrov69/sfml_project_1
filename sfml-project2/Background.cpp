#include "precompheaders.h"
#include "Background.h"

Background::Background()
{
	backgroundTexture.loadFromFile("IMAGES/background.jpg");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(1.f, 0.8f);
}

void Background::render(sf::RenderTarget& target)
{
	target.draw(backgroundSprite);
}
