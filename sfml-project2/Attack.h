#pragma once
#include "Bar.h"
#include "Math.h"
#include "Animation.h"

class Attack
{
private:
	Animation frame;
	
	float attackMoveSpeed;

	float attackScale;

	//Sprite for attack
	sf::Sprite m_Sprite;
	
	EActionState ac_state;

	EMovementState state;

	//Direction of attack, relative to the direction the player is facing
	float m_Direction;

	//The angle of the attack 
	float angle;

	//to describe
	sf::Vector2f shootDir;

	//Boolean for wether the attack is on-screen
	bool outOfBounds;

	void updateFrame();

	void initVariables();
public:
	//Default constructor, not meant to create the attack itself, use spawn() method to create an attack
	Attack(const sf::Texture& attack_texture, const int& attackWidth, const int& attackHeigth);

	Attack(const sf::Texture& attack_texture, const int& attackWidth, const int& attackHeigth, const float& nattackMoveSpeed, const float& scale);

	bool isFrameFinished() const;

	void setActionState(const EActionState&);

	const EActionState& getActionState() const;

	//Using offset angle will give you an entirely different movement vector that has the same origin as the main vector but is rotated by the angle
	void setShootDir(const sf::Vector2f&, const sf::Vector2f&, const float& offsetAngle = 0);

	//Changes the direction of the attack
	void changeDirection(const float&);

	//Spawn an relative to the argument sprite
	void spawn(const sf::Sprite&);

	//Updates the screen position of the attack and checks if out of bounds on render target
	void update(const sf::Vector2u& targetSize);

	//Renders attack sprite
	void render(sf::RenderTarget&);

	//Gets variable outOfBounds
	bool isOutOfBounds() const;

	const sf::FloatRect& getGlobalBounds() const;
};

