#pragma once
#include "precompheaders.h"

/// <summary>
/// Class healthbar
/// Creates a visual representation of the player's health
/// and it displays it over the player's head. The width of the healthbar
/// scales relative to the width of the player, the height is fixed.
/// update and render functions is all needed in order to use it.
/// Update takes the entity's sprite, its max health and its current health as arguments.
/// Render takes the RenderTarget for the screen on which the bar will be displayed.
/// </summary>
class Healthbar
{
private:
	float Percentage;

	sf::RectangleShape outline;
	sf::RectangleShape health;

	void initOutline();
	void initHealth();
	
	void updatePercentage(const float&, const float&);
	void updateSize(const sf::Sprite& sprite);
	void updatePosition(const sf::Sprite& sprite);
	void updateColor();
public:
	//Default constructor that does nothing
	Healthbar();

	//Updates the healthbar based on current sprite variables
	void update(const sf::Sprite&, const float& ,const float&);

	//Renders the healthbar on a given render target
	void render(sf::RenderTarget&);

};

