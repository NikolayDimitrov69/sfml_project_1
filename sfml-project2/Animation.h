#pragma once

//Playerstate will be used to easily track what the player is currently doing, will be useful with animations
enum Movementstate { IDLE = 0, MOVING, JUMPING, FALLING, NONE };

//Although it might seems like a boolean at first, this action state is made so that new actions can easily be added in the future
enum Actionstate { SHOOTING = 0, NOT_SHOOTING, DYING };

enum Physicstate { MID_AIR = 0, ON_GROUND, ON_SLOPE };

class Animation
{
private:
	sf::IntRect currentFrame;
	sf::Clock animTimer;
	int frameWidth;
	int frameHeigth;

	float idleSpeed;

	//We want to know to size of the texture, firstly because the death animation of every entity will be at the bottom of their spritesheet
	sf::Vector2u textureSize;

	bool restarted;

	bool finished;
public:
	Animation();

	bool isFinished() const;

	void setTextureSize(sf::Vector2u size);

	const sf::IntRect& getCurrentFrame() const;

	void IterateFrames(float seconds, unsigned numberOfFrames);

	void setIdleSpeed(float speed);

	void updateIdle();

	//Dying animation of every entity should be located at the bottom of the sprite sheet with the largest frame being one pixel above bottom
	//This will help us have spritesheets any size without having to manually set where the dying animation will be
	//Dying animation currentFrame.top will be equal to the texture.y size - framHeigth
	void updateDying();

	void updateMoving();

	void updateShooting();

	void updateShootingMoving();

	void updateJumping();

	void updateJumpingShooting();

	void setDimension(int, int);

	//Updater that updates the frame based on the movement and action states of an entity
	void update(const Movementstate&, const Actionstate&);
};

