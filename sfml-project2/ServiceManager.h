#pragma once

using VoidPtr = std::shared_ptr<void>;

class TypeIdGenerator {
public:
    template<typename T>
    static std::size_t Get() {
        static const std::size_t id = counter++;
        return id;
    }

private:
    static std::size_t counter;
};

std::size_t TypeIdGenerator::counter = 0;

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
        auto it = services.find(TypeIdGenerator::Get<T>());
        if (it != services.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr; // Service not found
    }

    void RegisterNeededServices();

private:
    // Register a service using its type
    template<typename T>
    void RegisterService() {
        services[TypeIdGenerator::Get<T>()] = std::make_shared<T>();
    }
    
    ServiceManager() = default;

    std::unordered_map<std::size_t, VoidPtr> services;
};

