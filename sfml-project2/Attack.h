#pragma once
#include "Healthbar.h"
#include "Math.h"

constexpr float ATTACK_MOVE_SPEED = 7.f;

class Attack
{
private:
	//Texture for attack
	sf::Texture m_Texture;
	
	//Sprite for attack
	sf::Sprite m_Sprite;
	
	//Direction of attack, relative to the direction the player is facing
	int m_Direction;

	//The angle of the attack 
	float angle;

	//to describe
	sf::Vector2f shootDir;

	//Boolean for wether the attack is on-screen
	bool outOfBounds;
public:
	//Default constructor, not meant to create the attack itself, use spawn() method to create an attack
	Attack();

	void setShootDir(const sf::Vector2f&, const sf::Vector2f&);

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

	const sf::FloatRect& getGlobalBounds() const;
};

