#pragma once

class Slope
{
private:
	sf::Texture slopeTexture;
	sf::Sprite slopeSprite;
	sf::Vector2f prev_pos;
	float fallSpeed;
public:
	Slope();

	const float& getSlopeTop() const;

	const float& getSlopeLeft() const;

	const float& getSlopeRight() const;

	sf::FloatRect getGlobalBounds() const;

	void setRandomVertPos(sf::RenderTarget&);

	void setFallSpeed(const float&);

	void update();

	void render(sf::RenderTarget&);

};

