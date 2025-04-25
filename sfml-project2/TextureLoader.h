#pragma once

class TextureLoader
{
public:
	TextureLoader();

private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> texture;
};

