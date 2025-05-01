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

int RandomNumber(int min, int max)
{
	return min + (std::rand() % (max - min + 1));
}
