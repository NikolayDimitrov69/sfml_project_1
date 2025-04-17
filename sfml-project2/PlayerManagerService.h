#pragma once
#include "Player.h"

class PlayerManagerService
{
public:
	PlayerManagerService();

	Player* GetPlayer() const;

	const sf::Sprite& GetSprite() const;

	sf::Sprite& MutableSprite();

	void InitializePlayer();
private:
	std::unique_ptr<Player> m_Player;
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
};

