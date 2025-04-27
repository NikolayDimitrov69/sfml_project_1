#pragma once
#include "ServiceManager.h"
class ServiceManager;
class Player;
class MainConfigLoader;

ServiceManager& GetServiceManager();

MainConfigLoader& GetConfigLoader();

template <typename Service>
std::shared_ptr<Service> GetService()
{
	return GetServiceManager().GetService<Service>();
}

Player* GetPlayerObject();

