#pragma once
#include "ServiceManager.h"
class ServiceManager;
class Player;
class MainConfigLoader;

template <typename T>
inline bool IsValidEnum(T val)
{
	ReturnIf((int)val > (int)T::NONE && (int)val < (int)T::COUNT, true);
	return false;
}

ServiceManager& GetServiceManager();

MainConfigLoader& GetConfigLoader();

template <typename Service>
inline std::shared_ptr<Service> GetService()
{
	return GetServiceManager().GetService<Service>();
}

Player* GetPlayerObject();

int RandomNumber(int min, int max);

