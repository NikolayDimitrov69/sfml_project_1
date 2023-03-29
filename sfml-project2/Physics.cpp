#include "precompheaders.h"
#include "Physics.h"

void Physics::initMoveVars()
{
	maxMoveVelocity = 3.f;
	minMoveVelocity = 1.f;
	moveAcceleration = 1.2f;
	moveDrag = 0.96f;
	gravity = 1.05f;
	maxFallVelocity = 5.f;
}

Physics::Physics()
{
	initMoveVars();
}

void Physics::setVelocity_X(const float& dir_x)
{
	moveVelocity.x += dir_x * moveAcceleration;
	
	if (std::abs(moveVelocity.x) > maxMoveVelocity)
		moveVelocity.x = maxMoveVelocity * dir_x;
}

void Physics::setVelocity_Y(const float& y)
{
	if (moveVelocity.y == 0)
		moveVelocity.y = -1.f * y;
}

void Physics::stopGravity()
{
	if(moveVelocity.y > 0)
		moveVelocity.y = 0;
}

const sf::Vector2f& Physics::getMoveVelocity() const
{
	return moveVelocity;
}

void Physics::updateGravity()
{
	if (moveVelocity.y < 0)
		moveVelocity.y = moveVelocity.y + 0.17f;
	else {
		if (moveVelocity.y < 1)
			moveVelocity.y += 1.f;

		moveVelocity.y *= gravity;
		if (moveVelocity.y > maxFallVelocity)
			moveVelocity.y = maxFallVelocity;
	}
}

void Physics::updateMovePhysics()
{
	moveVelocity.x *= moveDrag;
	if (std::abs(moveVelocity.x) < minMoveVelocity)
		moveVelocity.x = 0.f;
	if (std::abs(moveVelocity.y) < minMoveVelocity)
		moveVelocity.y = 0.f;
}
