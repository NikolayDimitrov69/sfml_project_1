#include "precompheaders.h"
#include "Gameover.h"
#include "Constants.h"

void Gameover::updateFrame()
{
	frame.updateIdle();
	go_sprite.setTextureRect(frame.getCurrentFrame());

	click_frame.updateIdle();
	click_sprite.setTextureRect(click_frame.getCurrentFrame());
}

Gameover::Gameover()
{
	click_texture.loadFromFile("IMAGES/click.png");
	click_sprite.setTexture(click_texture);
	click_sprite.setTextureRect(sf::IntRect(0, 0, click_texture.getSize().x / 2.f, click_texture.getSize().y));
	click_sprite.setOrigin(click_sprite.getGlobalBounds().width / 2.f, click_sprite.getGlobalBounds().height / 2.f);
	click_sprite.setScale(7.f, 7.f);
	click_frame.setDimension(click_texture.getSize().x / 2, click_texture.getSize().y);
	click_frame.setIdleSpeed(0.30f);
	click_frame.setNumberOfFrames(2);
	pressTimer = 0.f;
}

void Gameover::setTexture(const sf::Texture& texture, unsigned frames, float scale, sf::Vector2f pos)
{
	go_sprite.setTexture(texture);
	go_sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x / frames, texture.getSize().y));
	go_sprite.setOrigin(go_sprite.getGlobalBounds().width / 2.f, go_sprite.getGlobalBounds().height / 2.f);
	go_sprite.setScale(scale, scale);
	go_sprite.setPosition(pos);
	frame.setDimension(texture.getSize().x / frames, texture.getSize().y);
	frame.setIdleSpeed(0.15f);
	frame.setTextureSize(texture.getSize());
	frame.setNumberOfFrames(frames);
	click_sprite.setPosition(go_sprite.getPosition().x, go_sprite.getPosition().y + click_sprite.getGlobalBounds().height * 3.5f);
}

EGameState Gameover::update(const sf::Vector2f& mousePos, const sf::Vector2u& targetSize, EGameState state)
{
	pressTimer += 1.f;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && pressTimer >= MENU_PRESS_TO_CONTINUE_TIMER)
	{
		pressTimer = 0.f;
		if (state == EGameState::GameOver)
			return  EGameState::Home;
		if (state == EGameState::Home)
			return  EGameState::Playing;
		if (state == EGameState::StageComplete)
			return EGameState::Home;
	}
	updateFrame();
	return state;
}

void Gameover::render(sf::RenderTarget& target)
{
	target.draw(go_sprite);
	if (pressTimer >= MENU_PRESS_TO_CONTINUE_TIMER)
	{
		target.draw(click_sprite);
	}
}
