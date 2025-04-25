#pragma once
#include "TypeIdGenerator.h"

class GameObject
{
public:
	
	template <typename Component>
	std::shared_ptr<Component> get()
	{
		auto it = components.find(TypeIdGenerator::Get<Component>());
		if (it != components.end())
		{
			return std::static_pointer_cast<Component>(it->second);
		}
		return nullptr;
	}


private:
	std::unordered_map<std::size_t, std::shared_ptr<void>> components;
	friend class ObjectMutator;
};

class ObjectMutator
{
public:
	ObjectMutator(GameObject* obj);

	template <typename Component>
	ObjectMutator& add()
	{
		if (m_Obj.get<Component>())
		{
			assert(false && "component already exists!");
		}
		else
		{
			m_Obj.components[TypeIdGenerator::Get<Component>()] = std::make_shared<Component>();
		}
		return *this;
	}

	template <typename Component>
	ObjectMutator& remove()
	{
		m_Obj.components.erase(TypeIdGenerator::Get<Component>());
		return *this;
	}

private:
	GameObject& m_Obj;
};