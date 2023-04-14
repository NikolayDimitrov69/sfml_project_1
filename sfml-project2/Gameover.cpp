#include "precompheaders.h"
#include "Gameover.h"

void Gameover::updateFrame()
{
	frame.updateIdle();
	go_sprite.setTextureRect(frame.getCurrentFrame());
}

Gameover::Gameover()
{
	go_texture.loadFromFile("IMAGES/gameover.png");
	go_sprite.setTexture(go_texture);
	go_sprite.setTextureRect(sf::IntRect(0, 0, 134, 18));
	go_sprite.setOrigin(go_sprite.getLocalBounds().width / 2.f, go_sprite.getLocalBounds().height / 2.f + go_sprite.getLocalBounds().height / 2.f);
	go_sprite.setScale(7.f, 7.f);
	frame.setDimension(134, 18);
	frame.setIdleSpeed(0.15f);
	frame.setTextureSize(go_texture.getSize());
}

void Gameover::update(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize)
{
	updateFrame();
	go_sprite.setPosition(targetSize.x / 2.f, targetSize.y / 2.f);
}

void Gameover::render(sf::RenderTarget& target)
{
	target.draw(go_sprite);
}
