#include "precompheaders.h"
#include "GameEnvironmentService.h"
#include "EnemySpawnerController.h"

GameEnvironmentService::GameEnvironmentService()
{
    m_Window = new sf::RenderWindow(sf::VideoMode(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), "Game", sf::Style::Close | sf::Style::Titlebar);
    m_Window->setFramerateLimit(GAME_FPS);

    m_SystemObject = new GameObject();
    ObjectMutator(m_SystemObject)
        .add<EnemySpawnerController>()
        .apply();
}

GameObject* GameEnvironmentService::GetSystemObject() const
{
    return m_SystemObject;
}

sf::RenderWindow* GameEnvironmentService::GetWindow()
{
    return m_Window;
}

const sf::RenderWindow* GameEnvironmentService::GetWindow() const
{
    return m_Window;
}

GameEnvironmentService::~GameEnvironmentService()
{
    delete m_SystemObject;
    delete m_Window;
}
