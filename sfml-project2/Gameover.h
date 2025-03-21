#pragma once
#include "Animation.h"
#include "Pause.h"

class Gameover
{
private:
	sf::Sprite go_sprite;
	Animation frame;

	sf::Texture click_texture;
	sf::Sprite click_sprite;
	Animation click_frame;

	float pressTimer;

	void updateFrame();

public:
	Gameover();

	void setTexture(const sf::Texture& texture, unsigned frames, float scale, sf::Vector2f pos);

	EGameState update(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize, EGameState state);

	void render(sf::RenderTarget& target);

};

