#include "precompheaders.h"
#include "IConfigLoader.h"

void MainConfigLoader::Initialize()
{
    std::ifstream in(GAME_CONFIG_FILE_PATH);
    ThrowUnless(in.is_open(), std::runtime_error("Cannot open config file!"));
    json configJson;
    in >> configJson;
    updateLoaders(configJson);
}

void MainConfigLoader::updateLoaders(const json& data)
{
    for (auto& loader : loaders) {
        if (data.contains(loader.second.configName)) {
            loader.second.configPtr->Update(data.at(loader.second.configName));
        }
    }
}
