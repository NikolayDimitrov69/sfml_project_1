#include "precompheaders.h"
#include "ServiceManager.h"
#include "PlayerManagerService.h"

void ServiceManager::RegisterNeededServices()
{
	auto& serviceMgr = GetInstance();
	serviceMgr.RegisterService<PlayerManagerService>();


	InitializeServices();
}

void ServiceManager::InitializeServices()
{
	for (auto& service : services)
	{
		service.second->Initialize();
	}
}
