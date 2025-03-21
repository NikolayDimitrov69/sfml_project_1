#include "precompheaders.h"
#include "Item.h"

void Item::updateFrame()
{
	frame.updateIdle();
	itemSprite.setTextureRect(frame.getCurrentFrame());
}

Item::Item(const sf::Texture& texture, EItemType spec, float nspeed)
{
	itemSpec = spec;
	speed = nspeed;
	itemSprite.setTexture(texture);
	itemSprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x / 4, texture.getSize().y));
	itemSprite.setScale(3.5f, 3.5f);
	frame.setNumberOfFrames(4);
	frame.setDimension(texture.getSize().x / 4, texture.getSize().y);
	frame.setIdleSpeed(0.1f);
	frame.setTextureSize(texture.getSize());
	itemSprite.setOrigin(itemSprite.getLocalBounds().width / 2, itemSprite.getLocalBounds().height / 2);
}

void Item::setPosition(const sf::Vector2f& position)
{
	itemSprite.setPosition(position);
}

const sf::FloatRect& Item::getGlobalBounds() const
{
	return itemSprite.getGlobalBounds();
}

EItemType Item::getItemSpec() const
{
	return itemSpec;
}

void Item::update()
{
	updateFrame();
	itemSprite.move(0.f, speed);
}

void Item::render(sf::RenderTarget& target)
{
	target.draw(itemSprite);
}
