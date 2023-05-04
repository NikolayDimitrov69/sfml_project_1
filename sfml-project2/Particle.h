#pragma once
#include "Animation.h"

class Particle
{
private:
	sf::Sprite sprite;
	Animation frame;
	unsigned times;

public:
	Particle(const sf::Texture& texture, unsigned ntimes, unsigned frames, float scale, float speed);


};

