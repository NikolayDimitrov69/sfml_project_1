#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

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
public:
	//Default constructor that does nothing
	Healthbar();

	//Updates the healthbar based on current sprite variables
	void update(const sf::Sprite&, const float& ,const float&);

	//Renders the healthbar on a given render target
	void render(sf::RenderTarget&);

};

