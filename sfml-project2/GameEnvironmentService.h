#pragma once

class GameEnvironmentService
{
public:
	GameEnvironmentService();

	GameObject* GetSystemObject() const;

	~GameEnvironmentService();
private:

	GameObject* m_SystemObject;
};

