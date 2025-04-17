#include "precompheaders.h"
#include "EssentialFunctions.h"
#include "PlayerManagerService.h"

ServiceManager& GetServiceManager()
{
	return ServiceManager::GetInstance();
}

Player* GetPlayerObject()
{
    return GetService<PlayerManagerService>()->GetPlayer();
}