#include "precompheaders.h"
#include "GameEnvironmentService.h"

GameEnvironmentService::GameEnvironmentService()
{
    m_SystemObject = new GameObject();
}

GameObject* GameEnvironmentService::GetSystemObject() const
{
    return m_SystemObject;
}

GameEnvironmentService::~GameEnvironmentService()
{
    delete m_SystemObject;
}
