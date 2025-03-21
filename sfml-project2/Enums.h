#pragma once

enum class EMovementState : int
{ 
	INVALID = -1,
	NONE = 0,
	IDLE, 
	MOVING, 
	JUMPING, 
	FALLING, 
	COUNT
};

enum class EActionState : int
{ 
	INVALID = -1,
	NONE = 0,
	SHOOTING, 
	NOT_SHOOTING, 
	DYING 
};

enum class EPhysicState : int
{ 
	INVALID = -1,
	NONE = 0,
	MID_AIR,
	ON_GROUND,
	ON_SLOPE 
};

enum class EItemType : int
{
	INVALID = -1,
	NONE = 0,
	HEAL,
	DOUBLE_ATTACK,
	DOUBLE_JUMP,
	BOOST_ATTACK,
	COUNT
};

enum class EGameState : int
{
	INVALID = -1,
	NONE = 0,
	Menu, 
	Playing, 
	Paused, 
	GameOver, 
	StageComplete, 
	Home, 
	Restart, 
	Quit,
	COUNT
};