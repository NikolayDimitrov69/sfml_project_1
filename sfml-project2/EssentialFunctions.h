#pragma once
#include "ServiceManager.h"
class ServiceManager;
class Player;
class MainConfigLoader;

template <typename T>
inline bool IsValidEnum(T val)
{
	return (int)val > (int)T::Invalid && (int)val < (int)T::COUNT;
}

ServiceManager& GetServiceManager();

MainConfigLoader& GetConfigLoader();

template <typename Service>
inline std::shared_ptr<Service> GetService()
{
	return GetServiceManager().GetService<Service>();
}

Player* GetPlayerObject();

sf::RenderWindow* GetGameWindow();

GameObject* GetSystemObject();

int RandomNumber(int min, int max);

