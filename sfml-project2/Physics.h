#pragma once

class Physics
{
private:
	//move physics
	sf::Vector2f moveVelocity;
	float maxMoveVelocity;
	float minMoveVelocity;
	float moveAcceleration;
	float moveDrag;

	float gravity;
	float maxFallVelocity;

	float jumpVelocity;
	float maxJumpVelocity;

	void initMoveVars();
public:
	Physics();

	const sf::Vector2f& getMoveVelocity() const;

	void setVelocity_X(const float&);

	void setVelocity_Y(const float&);

	void stopGravity();

	void updateGravity();

	void updateMovePhysics();

};

