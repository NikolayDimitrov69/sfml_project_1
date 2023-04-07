#pragma once

constexpr float INIT_SLOPE_FALL_SPEED = 1.2f;

class Slope
{
private:
	sf::RenderTarget* target;

	sf::Texture slopeTexture;
	sf::Sprite slopeSprite;
	sf::Vector2f prev_pos;
	float fallSpeed;
public:
	Slope();

	const sf::FloatRect& getGlobalBounds() const;

	//Sets random vertical position of the next slope, based on the location of the previos slope.
	void setRandomVertPos(sf::RenderTarget&);

	void setFallSpeed(const float&);

	void update();

	void render(sf::RenderTarget&);

};

