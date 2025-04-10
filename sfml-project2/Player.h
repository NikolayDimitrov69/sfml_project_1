#pragma once
#include "Attack.h"
#include "Physics.h"
#include "Animation.h"

struct Playerhealth {

	Playerhealth(float health)
		: m_Health(health)
		, currentHealth(health)
	{
	}

	Bar healthbar;
	float m_Health;
	float currentHealth;
};

class Player
{
private:
	//Current playerstate
	EMovementState playerstate;

	//Current physics state, so the physic class knows which physic to apply to the player
	EPhysicState physicstate;

	//Keeps a track of the player's action state
	EActionState actionstate;

	//Player physics
	Physics playerphysics;

	float attackCooldown;

	//Player name
	std::string m_Name;

	//Player health
	Playerhealth health;

	//The player's damage
	float damage;

	//Count of doublejumps the player has
	unsigned doubleJumps;
	bool doubleJumpedOnce;
	float doubleJumpTimer;

	//Player attack, attack container and max attack counter
	std::vector<Attack> attacks;
	sf::Texture attack_texture;

	//Player's cooldown time and max cooldown
	float attCooldown;
	float boostAttackTimer;
	float doubleAttCooldown;

	//Player texture and sprite
	Animation frame;
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	//Key timers
	float timer;

	//Private initializer functions
	void initPlayerHealth();
	void initAttack();
	void initSprite();

	//Push_back attack whenever spawned
	void createSingleAttack(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize);
	void createDoubleAttack(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize);

	//Renderers
	void renderHealth(sf::RenderTarget& target);
	void renderSprite(sf::RenderTarget& target);
	void renderAttack(sf::RenderTarget& target);

	//Simple function for flipping the sprite left/right based on its current scale
	void TurnLeft();
	void TurnRight();

	void updateAttackCooldown();
	//Updaters
	void updateAttack(const sf::Vector2u& targetSize);
	//Updates the player's Bar sprite
	void updateHealth();
	//Updates the input taking in the mouse position as an argument
	void updateInputAndSates(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize);
	//Updates the player physics and moves the sprite of the player by the produced vector from class physics
	void updatePlayerPhysics();
	//Applies the playerphysics's updateGravity method
	void applyGravity();
	//Updates the frame
	void updateFrame();
public:

	Player();

	void heal(float amount);

	const float& dealDamage() const;

	void parentTo(float speed);

	//Iterates the attack vector and removes the attack which has hit an enemy, meant to be used by the game class each frame
	bool attackHasHit(const sf::FloatRect&);

	//Function for taking damage
	void takeDamage(float);

	int getDoubleJumps() const;

	void gainDoubleJump();

	//Gets the vector position of the player's sprite
	sf::Vector2f getPostion() const;

	//Gets the max health of the player
	const float& getMaxHealth() const;

	//Gets the current health of the player
	const float& getCurrentHealth() const;

	//Gets the global bounds of the player sprite
	sf::FloatRect getGlobalBounds() const;

	//Changes the physic state of the player, based on whats happening in the game
	void setPhysicState(const EPhysicState&);

	//Returns the physic state of the player
	const EPhysicState& getPhysState() const;

	//updates the physics vector's X variable;
	void move_x(const float&);

	//updates the physics vector's Y variable;
	void jump(const float&);

	void updateTimers();

	void resetBoostTimer();

	//updates the player and takes vector as an argument, which this game is the mouse position
	void updatePlayer(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize);

	//renders the player
	void renderPlayer(sf::RenderTarget& target);

	//Check if timer is above Jump cooldown
	bool keyPressable();

	void resetDoubleAttTimer();

	float getDoubleAttackTimer() const;

	float getBoostAttackTimer() const;

	void initialize();

	void setTexture(const sf::Texture& texture);
};
