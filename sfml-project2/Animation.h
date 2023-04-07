#pragma once

//Playerstate will be used to easily track what the player is currently doing, will be useful with animations
enum Movementstate { IDLE = 0, MOVING, JUMPING, FALLING };

//Although it might seems like a boolean at first, this action state is made so that new actions can easily be added in the future
enum Actionstate { SHOOTING = 0, NOT_SHOOTING };

enum Physicstate { MID_AIR = 0, ON_GROUND, ON_SLOPE };

class Animation
{
private:
	sf::IntRect currentFrame;
	sf::Clock animTimer;
	float frameWidth;
	float frameHeigth;

	float idleSpeed;
public:
	Animation();

	const sf::IntRect& getCurrentFrame() const;

	void IterateFrames(float seconds, unsigned numberOfFrames);

	void setIdleSpeed(float speed);

	void updateIdle();

	void updateMoving();

	void updateShooting();

	void updateShootingMoving();

	void updateJumping();

	void updateJumpingShooting();

	void setDimension(float, float);

	//Updater that updates the frame based on the movement and action states of an entity
	void update(const Movementstate&, const Actionstate&);
};

