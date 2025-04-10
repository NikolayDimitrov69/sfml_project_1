#pragma once
#include "Enums.h"

class Animation
{
private:
	sf::IntRect currentFrame;
	sf::Clock animTimer;
	int frameWidth;
	int frameHeigth;

	int numberOfFrames;

	float idleSpeed;

	//We want to know to size of the texture, firstly because the death animation of every entity will be at the bottom of their spritesheet
	sf::Vector2u textureSize;

	bool restarted;

	bool finished;
public:
	Animation();

	void setNumberOfFrames(int nframes);

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
	void update(const EMovementState&, const EActionState&);
};

