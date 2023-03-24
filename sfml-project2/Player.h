#pragma once
#include "Attack.h"

class Player
{
private:
	//Specifies the render target
	sf::RenderTarget& m_Target;

	//Player name and health
	std::string m_Name;
	int m_Health;
	float moveSpeed;

	//Player info font and text
	sf::Font font;
	sf::Text PlayerInfo;
	
	//Player attack, attack container and max attack counter
	std::vector<Attack> attacks;
	Attack attack;
	int maxAttacks;

	
	//Player texture and sprite
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	
	//Private initializer functions
	void initPlayerInfo();
	void initAttack();
	void initSprite(std::string& texture);

	//Push_back attack whenever spawned
	void fillAttackVector();

	//Renderers
	void renderPlayerInfo();
	void renderSprite();
	void renderAttack();

	//Updaters
	void updateAttack();
	void updatePlayerInfo();
	void updateInput();
public:
	
	Player(sf::RenderTarget& target ,std::string name = "newplayer", int health = 10, std::string texture = "IMAGES/trans_sprite_test.png");	

	const sf::Vector2f& getPostion() const;



	void updatePlayer();
	
	void renderPlayer();
};

