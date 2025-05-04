#pragma once

class GameEnvironmentService
{
public:
	GameEnvironmentService();

	GameObject* GetSystemObject() const;

	sf::RenderWindow* GetWindow();

	const sf::RenderWindow* GetWindow() const;

	~GameEnvironmentService();
private:

	GameObject* m_SystemObject;

	sf::RenderWindow* m_Window;
};

