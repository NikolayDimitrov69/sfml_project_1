#include "precompheaders.h"
#include "GameEnvironmentService.h"
#include "EnemySpawnerController.h"

GameEnvironmentService::GameEnvironmentService()
{
    m_SystemObject = new GameObject();
    ObjectMutator(m_SystemObject)
        .add<EnemySpawnerController>();
}

GameObject* GameEnvironmentService::GetSystemObject() const
{
    return m_SystemObject;
}

GameEnvironmentService::~GameEnvironmentService()
{
    delete m_SystemObject;
}
