#pragma once
#include "Attack.h"
#include "Physics.h"
#include "Animation.h"

//Playerstate will be used to easily track what the player is currently doing, will be useful with animations
enum Movementstate {IDLE = 0, MOVING, JUMPING, FALLING};

//Although it might seems like a boolean at first, this action state is made so that new actions can easily be added in the future
enum Actionstate {SHOOTING = 0, NOT_SHOOTING};

enum Physicstate {MID_AIR = 0, ON_GROUND, ON_SLOPE};

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
	Movementstate playerstate;

	//Current physics state, so the physic class knows which physic to apply to the player
	Physicstate physicstate;

	//Keeps a track of the player's action state, for now it only check whether the player is shooting or not
	Actionstate actionstate;

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
	Animation frame;
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	//Key timers
	sf::Clock timer;
	

	//Private initializer functions
	void initPlayerHealth();
	void initAttack();
	void initSprite(std::string& texture);

	//Push_back attack whenever spawned
	void fillAttackVector(const sf::Vector2f&);

	//Renderers
	void renderHealth();
	void renderSprite();
	void renderAttack();
	
	//Simple function for flipping the sprite left/right based on its current scale
	void TurnLeft();
	void TurnRight();

	//Updaters
	void updateAttack();
	//Updates the player's healthbar sprite
	void updateHealth();
	//Updates the input taking in the mouse position as an argument
	void updateInput(const sf::Vector2f&);
	//Updates the player physics and moves the sprite of the player by the produced vector from class physics
	void updatePlayerPhysics();
	//Applies the playerphysics's updateGravity method
	void applyGravity();
	//Updates the frame
	void updateFrame();
public:
	Player();

	Player(sf::RenderTarget* target ,std::string name = "newplayer", float newhealth = 10, std::string texture = "IMAGES/megaman.png");	

	void takeDamage(float);

	//Gets the vector position of the player's sprite
	const sf::Vector2f& getPostion() const;

	//Gets the max health of the player
	const float& getMaxHealth() const;

	//Gets the current health of the player
	const float& getCurrentHealth() const;

	//Gets the position of the top + height of the sprite, will be used by game class for collision check
	const float& getBottomHitbox() const;

	const float& getLeftHitbox() const;

	const float& getRightHitbox() const;

	const float& getPlayerWidth() const;

	sf::FloatRect getGlobalBounds() const;

	//Changes the physic state of the player, based on whats happening in the game
	void setPhysicState(const Physicstate&);

	const Physicstate& getPhysState() const;

	//updates the physics vector's X variable;
	void move_x(const float&);

	//updates the physics vector's Y variable;
	void jump(const float&);

	//updates the player and takes vector as an argument, which this game is the mouse position
	void updatePlayer(const sf::Vector2f&);
	
	//renders the player
	void renderPlayer();

	bool keyPressable();
};

