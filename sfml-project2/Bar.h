#pragma once
#include "precompheaders.h"

/// <summary>
/// Class Bar
/// Creates a visual representation of the player's health
/// and it displays it over the player's head. The width of the Bar
/// scales relative to the width of the player, the height is fixed.
/// update and render functions is all needed in order to use it.
/// Update takes the entity's sprite, its max health and its current health as arguments.
/// Render takes the RenderTarget for the screen on which the bar will be displayed.
/// </summary>
class Bar
{
private:
	float Percentage;

	sf::RectangleShape outline;
	sf::RectangleShape bar;

	void initOutline();
	void initColor();
	
	void updatePercentage(const float&, const float&);
	void updateSize();
	void updatePosition(const sf::Sprite& sprite);
	void updateColor();
public:
	//Default constructor that does nothing
	Bar();

	void setSize(const float& nwidth, const float& nheight);

	//Updates the Bar based on current sprite variables
	void update(const sf::Sprite&, const float& ,const float&);

	void setPosition(const sf::Vector2f& pos);

	//Used to update the bar without needing to have a sprite to follow
	void updateStatic(const float& maxValue, const float& currentValue);

	//Renders the Bar on a given render target
	void render(sf::RenderTarget&);

};

