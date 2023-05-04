#include "precompheaders.h"
#include "Pause.h"

Pause::Pause()
{
	pause_texture.loadFromFile("IMAGES/paused.png");
	pause_sprite.setTexture(pause_texture);
	pause_sprite.setScale(4.f, 4.f);
	pause_sprite.setOrigin(pause_sprite.getLocalBounds().width / 2.f, pause_sprite.getLocalBounds().height / 2.f + pause_sprite.getLocalBounds().height / 2.f);

	resume_texture.loadFromFile("IMAGES/resume_button.png");
	resume_sprite.setTexture(resume_texture);
	resume_sprite.setScale(2.f, 2.f);

	restart_texture.loadFromFile("IMAGES/restart_button.png");
	restart_sprite.setTexture(restart_texture);
	restart_sprite.setScale(2.f, 2.f);

	quit_texture.loadFromFile("IMAGES/quit_button.png");
	quit_sprite.setTexture(quit_texture);
	quit_sprite.setScale(2.f, 2.f);
}

Gamestate Pause::update(const sf::RenderTarget& rendertarget, const sf::Vector2f& mousePos)
{
	pause_sprite.setPosition(rendertarget.getSize().x / 2.f, rendertarget.getSize().y / 2.f);
	quit_sprite.setPosition(pause_sprite.getGlobalBounds().left, pause_sprite.getGlobalBounds().top + pause_sprite.getGlobalBounds().height + quit_sprite.getGlobalBounds().height / 4.f);
	resume_sprite.setPosition(pause_sprite.getGlobalBounds().left + pause_sprite.getGlobalBounds().width - resume_sprite.getGlobalBounds().width, pause_sprite.getGlobalBounds().top + pause_sprite.getGlobalBounds().height + resume_sprite.getGlobalBounds().height / 4.f);
	restart_sprite.setPosition(pause_sprite.getGlobalBounds().left + pause_sprite.getGlobalBounds().width / 2.f - restart_sprite.getGlobalBounds().width / 2.f, pause_sprite.getGlobalBounds().top + pause_sprite.getGlobalBounds().height + restart_sprite.getGlobalBounds().height / 4.f);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (resume_sprite.getGlobalBounds().contains(mousePos))
			return Gamestate::PLAYING;
		if (quit_sprite.getGlobalBounds().contains(mousePos))
			return Gamestate::HOME;
		if (restart_sprite.getGlobalBounds().contains(mousePos))
			return Gamestate::RESTART;
	}

	return Gamestate::PAUSED;
}

void Pause::render(sf::RenderTarget& rendertarget)
{
	rendertarget.draw(pause_sprite);
	rendertarget.draw(quit_sprite);
	rendertarget.draw(resume_sprite);
	rendertarget.draw(restart_sprite);
}
