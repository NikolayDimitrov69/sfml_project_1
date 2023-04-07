#pragma once
class Background
{
private:
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
public:
	Background();

	void render(sf::RenderTarget&);

};

