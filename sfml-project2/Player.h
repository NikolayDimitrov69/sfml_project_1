#pragma once
#include "Attack.h"
#include "Physics.h"

//Playerstate will be used to easily track what the player is currently doing, will be useful with animations
enum Playerstate {IDLE = 0, MOVE_RIGHT, MOVE_LEFT, JUMPING, FALLING, SHOOTING};

enum Physicstate {MID_AIR = 0, ON_GROUND};

struct Playerhealth {
	Healthbar healthbar;
	float m_Health;
	float currentHealth;
};

class Player
{
private:
	//Specifies the render target
	sf::RenderTarget* m_Target;

	//Current playerstate
	Playerstate playerstate;

	//Current physics state, so the physic class knows which physic to apply to the player
	Physicstate physicstate;

	//Player physics
	Physics playerphysics;

	//Player name and health
	std::string m_Name;
	Playerhealth health;
	
	//Player attack, attack container and max attack counter
	std::vector<Attack> attacks;
	Attack attack;
	int maxAttacks;

	//Player's cooldown time and max cooldown
	int maxAttCooldown;
	int attCooldown;
	
	//Player texture and sprite
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	
	//Private initializer functions
	void initPlayerHealth();
	void initAttack();
	void initSprite(std::string& texture);

	//Push_back attack whenever spawned
	void fillAttackVector(const sf::Vector2f&);

	//Renderers
	void renderPlayerInfo();
	void renderSprite();
	void renderAttack();
	
	//Simple function for flipping the sprite left/right based on its current scale
	void TurnLeft();
	void TurnRight();

	//Updaters
	void updateAttack();
	//Updates the player's healthbar sprite
	void updatePlayerInfo();
	//Updates the input taking in the mouse position as an argument
	void updateInput(const sf::Vector2f&);
	
	//Updates the player physics and moves the sprite of the player by the produced vector from class physics
	void updatePlayerPhysics();
	//Applies the playerphysics's updateGravity method
	void applyGravity();
public:
	Player();

	Player(sf::RenderTarget* target ,std::string name = "newplayer", float newhealth = 10, std::string texture = "IMAGES/trans_sprite_test.png");	

	//Gets the vector position of the player's sprite
	const sf::Vector2f& getPostion() const;

	//Gets the max health of the player
	float getMaxHealth() const;

	//Gets the current health of the player
	float getCurrentHealth() const;

	//Gets the position of the top + height of the sprite, will be used by game class for collision check
	float getBottomHitbox() const;

	//Changes the physic state of the player, based on whats happening in the game
	void setPhysicState(const Physicstate&);

	//updates the physics vector's X variable;
	void move_x(const float&);

	//updates the physics vector's Y variable;
	void jump(const float&);

	//updates the player and takes vector as an argument, which this game is the mouse position
	void updatePlayer(const sf::Vector2f&);
	
	//renders the player
	void renderPlayer();
};

