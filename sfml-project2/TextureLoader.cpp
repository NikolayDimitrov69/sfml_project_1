#include "precompheaders.h"
#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
}

void TextureLoader::Update(const json& data)
{
	json texturesArrJson;
	ReturnUnless(JsonParse(data, "textures", texturesArrJson));
	ReturnUnless(texturesArrJson.is_array());

	for (const auto& textureJson : texturesArrJson)
	{
		std::string name;
		ContinueUnless(JsonParse(textureJson, "name", name));
		std::string path;
		ContinueUnless(JsonParse(textureJson, "path", path));

		TexturePtr texture(new sf::Texture);
		AssertContinueUnless(texture->loadFromFile(path) && "Failed to load texture");

		AssertContinueIf(textures.find(name) != textures.end());
		textures[name] = std::move(texture);
	}
}

const sf::Texture* TextureLoader::GetTexture(const std::string& id) const
{
	auto it = textures.find(id);
	if (it != textures.end())
	{
		return it->second.get();
	}
	return nullptr;
}
