#pragma once
#include "IService.h"

class ServiceManager {
public:
    // Singleton instance
    static ServiceManager& GetInstance() {
        static ServiceManager instance;
        return instance;
    }

    // Retrieve a service by type
    template<typename T>
    std::shared_ptr<T> GetService() {
        auto it = services.find(typeid(T));
        if (it != services.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr; // Service not found
    }

    void RegisterNeededServices();

private:

    void InitializeServices();

    // Register a service using its type
    template<typename T>
    void RegisterService() {
        services[typeid(T)] = std::make_shared<T>();
    }

    ServiceManager() = default;

    std::unordered_map<std::type_index, std::shared_ptr<IService>> services;
};

