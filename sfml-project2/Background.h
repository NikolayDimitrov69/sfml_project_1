#pragma once
class Background
{
private:
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
public:
	Background();

	void setTexture(const std::string&);

	void setScale(const sf::Vector2f&);

	void render(sf::RenderTarget&);

};

