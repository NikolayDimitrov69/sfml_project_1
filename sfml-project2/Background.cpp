#include "precompheaders.h"
#include "Background.h"
#include "TextureLoader.h"
Background::Background()
{}

void Background::Init()
{
	auto texture = GetConfigLoader().Get<TextureLoader>()->GetTexture("background");
	backgroundSprite.setTexture(*texture);
}

void Background::setScale(const sf::Vector2f& scale)
{
	backgroundSprite.setScale(scale);
}

void Background::render(sf::RenderTarget& target)
{
	target.draw(backgroundSprite);
}
