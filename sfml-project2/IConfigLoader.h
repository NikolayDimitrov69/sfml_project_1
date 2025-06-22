#pragma once

class IConfigLoader {
public:
    virtual ~IConfigLoader() = default;
    virtual void Update(const json& section) = 0;
};

using ConfigPtr = std::shared_ptr<IConfigLoader>;

struct ConfigLoaderData {
    std::string configName;
    ConfigPtr configPtr;
};

class MainConfigLoader {
public:
    void Initialize();

    template <typename T>
    void registerLoader(const std::string& configName)
    {
        auto loader = std::make_shared<T>();
        
        ConfigLoaderData data;
        data.configName = configName;
        data.configPtr = std::move(loader);

        loaders[TypeIdGenerator::Get<T>()] = std::move(data);
    }

    template <typename T>
    std::shared_ptr<T> Get()
    {
        auto it = loaders.find(TypeIdGenerator::Get<T>());
        if (it != loaders.end()) {
            return std::static_pointer_cast<T>(it->second.configPtr);
        }
        return nullptr; // Service not found
    }

    DECLARE_SINGLETON(MainConfigLoader);

private:
    void updateLoaders(const json& data);

private:
    std::unordered_map<std::size_t, ConfigLoaderData> loaders;
};

template<typename T>
class StaticLoaderRegistration {
public:
    StaticLoaderRegistration(const std::string& configName) {
        static_assert(std::is_base_of<IConfigLoader, T>::value);
        GetConfigLoader().registerLoader<T>(configName);
    }
};

#define REGISTER_CONFIG_LOADER(LoaderType, ConfigName) \
    static StaticLoaderRegistration<LoaderType> _static_reg_##LoaderType(ConfigName)

template <typename T>
inline bool JsonParse(const json& data, const std::string& key, T& out)
{
    auto it = data.find(key);
    if (it != data.end())
    {
        out = it.value();
        return true;
    }
    return false;
}