#pragma once
#include "Enums.h"
#include "Animation.h"

class Item
{
private:
	sf::Sprite itemSprite;
	EItemType itemType;
	Animation frame;
	float speed;

	void updateFrame();
public:

	Item() = default;

	Item(const sf::Texture& texture, EItemType spec, float nspeed);

	void setPosition(const sf::Vector2f& position);

	sf::FloatRect getGlobalBounds() const;
	
	EItemType GetItemType() const;

	void update();

	void render(sf::RenderTarget& target);

};

