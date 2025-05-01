#include "precompheaders.h"
#include "ServiceManager.h"
#include "PlayerManagerService.h"
#include "TextureLoader.h"
#include "GameEnvironmentService.h"

void ServiceManager::RegisterNeededServices()
{
	RegisterService<PlayerManagerService>();
	RegisterService<GameEnvironmentService>();
}
