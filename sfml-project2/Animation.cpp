#include "precompheaders.h"
#include "Animation.h"

Animation::Animation()
{
	currentFrame = sf::IntRect(0, 0, 28, 25);
	animTimer.restart();
}

const sf::IntRect& Animation::getCurrentFrame() const
{
	return currentFrame;
}

void Animation::IterateFrames(float seconds, unsigned numberOfFrames, float frameWidth)
{
	if (animTimer.getElapsedTime().asSeconds() >= seconds)
	{
		currentFrame.left += frameWidth;
		if (currentFrame.left >= numberOfFrames * 28.f)
			currentFrame.left = 0.f;
		animTimer.restart();
	}
}

void Animation::updateIdle()
{
	currentFrame.top = 0;
	IterateFrames(0.5f, 3, 28.f);
}

void Animation::updateMoving()
{
	currentFrame.top = 25;
	IterateFrames(0.1f, 3, 28.f);
}

void Animation::updateShooting()
{
	currentFrame.top = 50;
	IterateFrames(0.08f, 3, 28.f);
}

void Animation::updateShootingMoving()
{
	currentFrame.top = 75;
	IterateFrames(0.1f, 3, 28.f);
}

void Animation::updateJumping()
{
	currentFrame.top = 100;
	IterateFrames(0.3f, 3, 28.f);
}

void Animation::updateJumpingShooting()
{
	currentFrame.top = 125;
	IterateFrames(0.3f, 3, 28.f);
}

