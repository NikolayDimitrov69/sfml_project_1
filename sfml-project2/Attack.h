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

class Attack
{
private:
	//Texture for attack
	sf::Texture m_Texture;
	
	//Sprite for attack
	sf::Sprite m_Sprite;
	
	//Direction of attack, relative to the direction the player is facing
	int m_Direction;

	//Boolean for wether the attack is on-screen
	bool outOfBounds;
public:
	//Default constructor, not meant to create the attack itself, use spawn() method to create an attack
	Attack();

	//Sets the texture from a given path
	void setTexture(const std::string&);

	//Changes the direction of the attack
	void changeDirection(const int&);

	//Spawn an relative to the argument sprite
	void spawn(const sf::Sprite&);

	//Updates the screen position of the attack and checks if out of bounds on render target
	void update(sf::RenderTarget&);

	//Renders attack sprite
	void render(sf::RenderTarget&);

	//Gets variable outOfBounds
	bool isOutOfBounds() const;
};
