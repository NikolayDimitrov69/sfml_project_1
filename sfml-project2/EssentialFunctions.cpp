#include "precompheaders.h"
#include "EssentialFunctions.h"
#include "ServiceManager.h"
#include "PlayerManagerService.h"

ServiceManager& GetServiceManager()
{
	return ServiceManager::GetInstance();
}

Player* GetPlayerObject()
{
    return GetServiceManager().GetService<PlayerManagerService>()->GetPlayer();
}