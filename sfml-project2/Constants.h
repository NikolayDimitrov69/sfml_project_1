#pragma once

// GAME constants
constexpr float		GAME_PROGRESSION_PER_FRAME = 0.01f;
constexpr float		GAME_SLOPE_SPAWN_TIMER = 225.f;
constexpr float		GAME_ITEM_SLOPE_SPAWN_TIMER = 1000.f;
constexpr float		GAME_INIT_SLOPE_FALL_SPEED = 1.2f;
constexpr int		GAME_WINDOW_WIDTH = 1600;
constexpr int		GAME_WINDOW_HEIGHT = 800;
constexpr float     GAME_WINDOW_SCALE = 2;

// MENU constants
constexpr float		MENU_PRESS_TO_CONTINUE_TIMER = 250.f;

// PLAYER constants
constexpr float		PLAYER_MAX_HEALTH = 100.f;
constexpr float		PLAYER_DEFAULT_ATTACK_MOVE_SPEED = 7.f;
constexpr float		PLAYER_ATTACK_COOLDOWN = 30.f;
constexpr float		PLAYER_JUMP_COOLDOWN = 1.5f;
constexpr float		PLAYER_JUMP_FORCE = 9.f;
constexpr float		PLAYER_DAMAGE = 30.f;
constexpr float		PLAYER_DOUBLE_ATTACK_TIMER = 800.f;
constexpr unsigned	PLAYER_MAX_DOUBLE_JUMPS = 3;
constexpr float		PLAYER_MIN_DOUBLE_JUMP_TIMER = 60.f;
constexpr float		PLAYER_BOOST_ATTACK_TIMER = 800.f;

// ENEMY constants
constexpr float		ENEMY_MAX_IMMUNITY_TIMER = 0.f;
constexpr float		ENEMY_RANGED_SPEED = 1.f;
constexpr float		ENEMY_RANGED_DAMAGE = 10.f;
constexpr float		ENEMY_RANGED_MAX_HEALTH = 50.f;
constexpr float		ENEMY_RANGED_ATTACK_COOLDOWN = 300.f;
constexpr float		ENEMY_RANGED_MOVING_TIMER = 400.f;
constexpr float		ENEMY_SPEED = 1.5f;
constexpr float		ENEMY_DAMAGE = 20.f;
constexpr float		ENEMY_MAX_HEALTH = 100.f;
constexpr float		ENEMY_SPAWN_TIMER = 200.f;

// UTILITY constants
constexpr double	UTILITY_M_PI = 3.141592653589793;
