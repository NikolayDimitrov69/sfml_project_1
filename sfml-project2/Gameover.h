#pragma once
#include "Animation.h"

class Gameover
{
private:
	sf::Texture go_texture;
	sf::Sprite go_sprite;
	Animation frame;

	void updateFrame();

public:
	Gameover();

	void update(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize);

	void render(sf::RenderTarget& target);

};

