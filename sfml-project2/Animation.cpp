#include "precompheaders.h"
#include "Animation.h"

Animation::Animation()
{
	restarted = false;
	finished = false;
	animTimer.restart();
}

bool Animation::isFinished() const
{
	return restarted && finished;
}

void Animation::setTextureSize(sf::Vector2u size)
{
	textureSize = size;
}

const sf::IntRect& Animation::getCurrentFrame() const
{
	return currentFrame;
}

void Animation::setDimension(int width, int heigth)
{
	frameWidth = width;
	frameHeigth = heigth;
	currentFrame = sf::IntRect(0, 0, frameWidth, frameHeigth);
}

void Animation::IterateFrames(float seconds, unsigned numberOfFrames)
{
	if (animTimer.getElapsedTime().asSeconds() >= seconds)
	{
		currentFrame.left += frameWidth;
		if (currentFrame.left >= numberOfFrames * frameWidth) {
			currentFrame.left = 0.f;
			finished = true;
		}
		else
		{
			finished = false;
		}
		animTimer.restart();
	}
}

void Animation::setIdleSpeed(float speed)
{
	idleSpeed = speed;
}

void Animation::updateIdle()
{
	currentFrame.top = 0;
	IterateFrames(idleSpeed, 3.f);
}

void Animation::updateDying()
{
	currentFrame.top = textureSize.y - frameHeigth;
	if (!restarted)
	{
		finished = false;
		currentFrame.left = 0.f;
		restarted = true;		
	}
	IterateFrames(0.15f, 3.f);
}

void Animation::updateMoving()
{
	currentFrame.top = frameHeigth;
	IterateFrames(0.1f, 3.f);
}

void Animation::updateShooting()
{
	currentFrame.top = frameHeigth * 2;
	IterateFrames(0.08f, 3.f);
}

void Animation::updateShootingMoving()
{
	currentFrame.top = frameHeigth * 3;
	IterateFrames(0.1f, 3.f);
}

void Animation::updateJumping()
{
	currentFrame.top = frameHeigth * 4;
	IterateFrames(0.3f, 3.f);
}

void Animation::updateJumpingShooting()
{
	currentFrame.top = frameHeigth * 5;
	IterateFrames(0.3f, 3.f);
}

void Animation::update(const Movementstate& mv_state, const Actionstate& ac_state)
{
	if (ac_state == Actionstate::DYING)
		updateDying();
	else if (mv_state == Movementstate::IDLE)
		updateIdle();
	else if (mv_state == Movementstate::MOVING)
		updateMoving();
	if (ac_state == Actionstate::SHOOTING && mv_state != Movementstate::MOVING)
		updateShooting();
	else if (ac_state == Actionstate::SHOOTING && mv_state == Movementstate::MOVING)
		updateShootingMoving();
	if ((mv_state == Movementstate::JUMPING || mv_state == Movementstate::FALLING) && ac_state != Actionstate::SHOOTING)
		updateJumping();
	else if ((mv_state == Movementstate::JUMPING || mv_state == Movementstate::FALLING) && ac_state == Actionstate::SHOOTING)
		updateJumpingShooting();
}

