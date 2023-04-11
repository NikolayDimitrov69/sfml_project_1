#pragma once
#include "Attack.h"
#include "Physics.h"
#include "Animation.h"

constexpr float ATTACK_COOLDOWN = 50.f; 
constexpr float JUMP_COOLDOWN = 1.5f; 
constexpr float JUMP_FORCE = 9.f;
constexpr float PLAYER_DAMAGE = 34.f;

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

	float damage;
	
	//Player attack, attack container and max attack counter
	std::vector<Attack> attacks;
	Attack attack;

	//Player's cooldown time and max cooldown
	float attCooldown;
	
	//Player texture and sprite
	Animation frame;
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	//Key timers
	float timer;
	
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
	void updateInputAndSates(const sf::Vector2f&);
	//Updates the player physics and moves the sprite of the player by the produced vector from class physics
	void updatePlayerPhysics();
	//Applies the playerphysics's updateGravity method
	void applyGravity();
	//Updates the frame
	void updateFrame();
public:
	Player();

	Player(sf::RenderTarget* target ,std::string name = "newplayer", float newhealth = 10, std::string texture = "IMAGES/megaman.png");	

	const float& dealDamage() const;

	//Iterates the attack vector and removes the attack which has hit an enemy, meant to be used by the game class each frame
	bool iterateAttackVector(const sf::FloatRect&);

	//Function for taking damage
	void takeDamage(float);

	//Gets the vector position of the player's sprite
	const sf::Vector2f& getPostion() const;

	//Gets the max health of the player
	const float& getMaxHealth() const;

	//Gets the current health of the player
	const float& getCurrentHealth() const;

	//Gets the global bounds of the player sprite
	sf::FloatRect getGlobalBounds() const;

	//Changes the physic state of the player, based on whats happening in the game
	void setPhysicState(const Physicstate&);

	//Returns the physic state of the player
	const Physicstate& getPhysState() const;

	//updates the physics vector's X variable;
	void move_x(const float&);

	//updates the physics vector's Y variable;
	void jump(const float&);

	//updates the player and takes vector as an argument, which this game is the mouse position
	void updatePlayer(const sf::Vector2f&);
	
	//renders the player
	void renderPlayer();

	//Check if timer is above Jump cooldown
	bool keyPressable();
};

