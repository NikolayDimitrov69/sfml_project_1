#pragma once

class Animation
{
private:
	sf::IntRect currentFrame;
	sf::Clock animTimer;
public:
	Animation();

	const sf::IntRect& getCurrentFrame() const;

	void IterateFrames(float seconds, unsigned numberOfFrames, float frameWidth);

	void updateIdle();

	void updateMoving();

	void updateShooting();

	void updateShootingMoving();

	void updateJumping();

	void updateJumpingShooting();
};

