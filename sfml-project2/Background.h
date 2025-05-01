#pragma once
class Background
{
private:
	sf::Sprite backgroundSprite;
public:
	Background();

	void Init();

	void setScale(const sf::Vector2f&);

	void render(sf::RenderTarget&);

};

