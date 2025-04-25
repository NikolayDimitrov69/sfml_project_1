#include "precompheaders.h"
#include "GameObject.h"

ObjectMutator::ObjectMutator(GameObject* obj)
	: m_Obj(*obj)
{
	assert(obj);
}
