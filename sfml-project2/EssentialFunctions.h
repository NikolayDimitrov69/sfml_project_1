#pragma once
#include "ServiceManager.h"
class ServiceManager;
class Player;

ServiceManager& GetServiceManager();

template <typename Service>
std::shared_ptr<Service> GetService()
{
	return GetServiceManager().GetService<Service>();
}

Player* GetPlayerObject();

