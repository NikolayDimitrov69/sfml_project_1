#pragma once
#include "IConfigLoader.h"

using TexturePtr = std::unique_ptr<sf::Texture>;

class TextureLoader : public IConfigLoader
{
public:
	TextureLoader();

	virtual void Update(const json& data) override;

	const sf::Texture* GetTexture(const std::string& id) const;

private:
	std::unordered_map<std::string, TexturePtr> textures;
};

REGISTER_CONFIG_LOADER(TextureLoader, "textureConfig");