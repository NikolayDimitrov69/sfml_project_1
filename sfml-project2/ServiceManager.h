#pragma once
#include "TypeIdGenerator.h"

using VoidPtr = std::shared_ptr<void>;

class ServiceManager {
public:
    // Retrieve a service by type
    template<typename T>
    std::shared_ptr<T> GetService() {
        auto it = services.find(TypeIdGenerator::Get<T>());
        if (it != services.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr; // Service not found
    }

    void RegisterNeededServices();

    DECLARE_SINGLETON(ServiceManager);
private:
    // Register a service using its type
    template<typename T>
    void RegisterService() {
        services[TypeIdGenerator::Get<T>()] = std::make_shared<T>();
    }
    
    

    std::unordered_map<std::size_t, VoidPtr> services;
};

