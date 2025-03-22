#pragma once
#include "IService.h"
#include "Player.h"

class PlayerManagerService : public IService
{
public:
	PlayerManagerService();

	Player* GetPlayer() const;

	virtual void Initialize() override;

	void InitializePlayer();
private:
	std::unique_ptr<Player> m_Player;
	sf::Texture m_PlayerTexture;
};

