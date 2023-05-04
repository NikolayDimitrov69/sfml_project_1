#include "precompheaders.h"
#include "Particle.h"

Particle::Particle(const sf::Texture& texture, unsigned ntimes, unsigned frames, float scale, float speed) : times(ntimes)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x / frames, texture.getSize().y));
	sprite.setScale(scale, scale);
	frame.setNumberOfFrames(frames);
	frame.setDimension(texture.getSize().x / frames, texture.getSize().y);
	frame.setIdleSpeed(speed);
	frame.setTextureSize(texture.getSize());
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
}
