#include "precompheaders.h"
#include "ServiceManager.h"
#include "PlayerManagerService.h"

void ServiceManager::RegisterNeededServices()
{
	RegisterService<PlayerManagerService>();
}
