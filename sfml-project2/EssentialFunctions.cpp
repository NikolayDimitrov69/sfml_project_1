#include "precompheaders.h"
#include "EssentialFunctions.h"
#include "PlayerManagerService.h"
#include "IConfigLoader.h"
#include "GameEnvironmentService.h"

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

sf::RenderWindow* GetGameWindow()
{
	return GetService<GameEnvironmentService>()->GetWindow();
}

GameObject* GetSystemObject()
{
	return GetService<GameEnvironmentService>()->GetSystemObject();
}

int RandomNumber(int min, int max)
{
	return min + (std::rand() % (max - min + 1));
}
