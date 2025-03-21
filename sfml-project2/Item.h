#pragma once
#include "Enums.h"
#include "Animation.h"

class Item
{
private:
	sf::Sprite itemSprite;
	EItemType itemSpec;
	Animation frame;
	float speed;

	void updateFrame();
public:

	Item() = default;

	Item(const sf::Texture& texture, EItemType spec, float nspeed);

	void setPosition(const sf::Vector2f& position);

	const sf::FloatRect& getGlobalBounds() const;
	
	EItemType getItemSpec() const;

	void update();

	void render(sf::RenderTarget& target);

};

