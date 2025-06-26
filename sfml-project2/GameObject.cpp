#include "precompheaders.h"
#include "GameObject.h"

ObjectMutator::ObjectMutator(GameObject* obj)
	: m_Obj(*obj)
	, m_Mutation(nullptr)
{
	AssertReturnUnless(obj);
	reset();
}

ObjectMutator::~ObjectMutator()
{
	if (m_Mutation)
	{
		delete m_Mutation;
	}
}

void ObjectMutator::reset()
{
	if (m_Mutation)
	{
		delete m_Mutation;
	}
	m_Mutation = new GameObject();
	m_Mutation->components = m_Obj.components;
	m_MutationDependancies.clear();
}

void ObjectMutator::apply()
{
	if (m_Mutation)
	{
		m_Obj.components = std::move(m_Mutation->components);
	}
	applyStoredDependancies();
	reset();
}

void ObjectMutator::applyStoredDependancies()
{
	for (const auto& depFn : m_MutationDependancies)
	{
		depFn(*this);
	}
}
