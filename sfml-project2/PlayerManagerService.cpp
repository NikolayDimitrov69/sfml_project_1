#include "precompheaders.h"
#include "PlayerManagerService.h"

PlayerManagerService::PlayerManagerService()
{
	InitializePlayer();
}

Player* PlayerManagerService::GetPlayer() const
{
	return m_Player.get();
}

void PlayerManagerService::Initialize()
{
	m_PlayerTexture.loadFromFile("IMAGES/megaman.png");
	m_Player->setTexture(m_PlayerTexture);
	m_Player->initialize();
}

void PlayerManagerService::InitializePlayer()
{
	m_Player.reset(new Player());
}