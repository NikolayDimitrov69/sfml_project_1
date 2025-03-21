#pragma once
#include "Enums.h"

class Pause
{
	sf::Texture pause_texture;
	sf::Sprite pause_sprite;

	sf::Texture resume_texture;
	sf::Sprite resume_sprite;

	sf::Texture restart_texture;
	sf::Sprite restart_sprite;

	sf::Texture quit_texture;
	sf::Sprite quit_sprite;
public:
	Pause();

	EGameState update(const sf::RenderTarget& rendertarget,const sf::Vector2f& mousePos);

	void render(sf::RenderTarget& rendertarget);
};

