#include "precompheaders.h"
#include "ServiceManager.h"
#include "PlayerManagerService.h"
#include "TextureLoader.h"

void ServiceManager::RegisterNeededServices()
{
	RegisterService<PlayerManagerService>();
}
