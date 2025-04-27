#include "precompheaders.h"
#include "EssentialFunctions.h"
#include "PlayerManagerService.h"
#include "IConfigLoader.h"

ServiceManager& GetServiceManager()
{
	return GetSingletonInstance<ServiceManager>();
}

MainConfigLoader& GetConfigLoader()
{
	return GetSingletonInstance<MainConfigLoader>();
}

Player* GetPlayerObject()
{
    return GetService<PlayerManagerService>()->GetPlayer();
}