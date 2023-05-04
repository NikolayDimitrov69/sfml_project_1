#pragma once
#include "Animation.h"
enum Itemspec {HEAL = 0, DOUBLE_ATTACK, DOUBLE_JUMP, BOOST_ATTACK};

class Item
{
private:
	sf::Sprite itemSprite;
	Itemspec itemSpec;
	Animation frame;
	float speed;

	void updateFrame();
public:

	Item() = default;

	Item(const sf::Texture& texture, Itemspec spec, float nspeed);

	void setPosition(const sf::Vector2f& position);

	const sf::FloatRect& getGlobalBounds() const;
	
	Itemspec getItemSpec() const;

	void update();

	void render(sf::RenderTarget& target);

};

