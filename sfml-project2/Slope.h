#pragma once

constexpr float INIT_SLOPE_FALL_SPEED = 1.2f;

class Slope
{
private:
	sf::Sprite slopeSprite;
	float fallSpeed;
public:
	Slope(const sf::Texture& texture);

	void setPostion(const sf::Vector2f& pos);

	void setScale(const sf::Vector2f& size);

	const sf::Vector2f& getPosition() const;

	const sf::FloatRect& getGlobalBounds() const;

	//Sets random vertical position of the next slope, based on the location of the previos slope.
	void setRandomVertPos(const sf::Vector2u& targetSize, sf::Vector2f& prev_pos);

	float getFallSpeed() const;

	void setFallSpeed(const float&);

	void update();

	void render(sf::RenderTarget&);

};

