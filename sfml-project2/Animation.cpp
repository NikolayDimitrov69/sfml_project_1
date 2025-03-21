#include "precompheaders.h"
#include "Animation.h"

Animation::Animation()
	: restarted(false)
	, finished(false)
	, idleSpeed(0.f)
	, frameHeigth(0)
	, frameWidth(0)
	, numberOfFrames(0)
{
	animTimer.restart();
}

void Animation::setNumberOfFrames(int nframes)
{
	numberOfFrames = nframes;
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
	IterateFrames(idleSpeed, numberOfFrames);
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
	IterateFrames(0.15f, numberOfFrames);
}

void Animation::updateMoving()
{
	currentFrame.top = frameHeigth;
	IterateFrames(0.1f, numberOfFrames);
}

void Animation::updateShooting()
{
	currentFrame.top = frameHeigth * 2;
	IterateFrames(0.08f, numberOfFrames);
}

void Animation::updateShootingMoving()
{
	currentFrame.top = frameHeigth * 3;
	IterateFrames(0.1f, numberOfFrames);
}

void Animation::updateJumping()
{
	currentFrame.top = frameHeigth * 4;
	IterateFrames(0.3f, numberOfFrames);
}

void Animation::updateJumpingShooting()
{
	currentFrame.top = frameHeigth * 5;
	IterateFrames(0.3f, numberOfFrames);
}

void Animation::update(const EMovementState& mv_state, const EActionState& ac_state)
{
	if (ac_state == EActionState::DYING)
		updateDying();
	else if (mv_state == EMovementState::IDLE)
		updateIdle();
	else if (mv_state == EMovementState::MOVING)
		updateMoving();
	if (ac_state == EActionState::SHOOTING && mv_state == EMovementState::IDLE)
		updateShooting();
	else if (ac_state == EActionState::SHOOTING && mv_state == EMovementState::MOVING)
		updateShootingMoving();
	if ((mv_state == EMovementState::JUMPING || mv_state == EMovementState::FALLING) && ac_state != EActionState::SHOOTING)
		updateJumping();
	else if ((mv_state == EMovementState::JUMPING || mv_state == EMovementState::FALLING) && ac_state == EActionState::SHOOTING)
		updateJumpingShooting();
}

