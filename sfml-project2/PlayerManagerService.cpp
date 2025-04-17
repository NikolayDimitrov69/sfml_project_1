#include "precompheaders.h"
#include "PlayerManagerService.h"
#include "Constants.h"

PlayerManagerService::PlayerManagerService()
{
}

Player* PlayerManagerService::GetPlayer() const
{
	return m_Player.get();
}

const sf::Sprite& PlayerManagerService::GetSprite() const
{
	return m_Sprite;
}

sf::Sprite& PlayerManagerService::MutableSprite()
{
	return m_Sprite;
}

void PlayerManagerService::InitializePlayer()
{
	m_Texture.loadFromFile("IMAGES/megaman.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, 28, 25));
	m_Sprite.setScale(4.f, 4.f);
	m_Sprite.setOrigin(m_Sprite.getLocalBounds().width / 2, m_Sprite.getLocalBounds().height / 2);
	m_Sprite.setPosition(GAME_WINDOW_WIDTH / 2.f, GAME_WINDOW_HEIGHT / 2.f);

	m_Player.reset(new Player());
	m_Player->initialize();
}