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
	ObjectMutator& add();

	template <typename Component>
	ObjectMutator& add(const std::shared_ptr<Component>& component);

	template <typename Component>
	ObjectMutator& add(std::shared_ptr<Component>&& component);

	template <typename Component>
	ObjectMutator& remove();

private:
	GameObject& m_Obj;
};

class ComponentDependancyRules
{
public:
	using DependancyFunction = std::function<void(ObjectMutator&)>;
	
	template <typename Dependee, typename Depender>
	void push()
	{
		DependancyFunction dpFn = [](ObjectMutator& mut)
			{
				mut.add<Depender>();
			};
		dependanciesMap[TypeIdGenerator::Get<Dependee>()].push_back(dpFn);
	}
	
	template <typename Dependee>
	void applyDependancies(ObjectMutator& mut)
	{
		auto it = dependanciesMap.find(TypeIdGenerator::Get<Dependee>());
		if (it != dependanciesMap.end())
		{
			for (const auto& dpFn : it->second)
			{
				dpFn(mut);
			}
		}
	}
	
	DECLARE_SINGLETON(ComponentDependancyRules);
private:
	std::unordered_map<std::size_t, std::vector<DependancyFunction>> dependanciesMap;
};

template <typename Component>
inline ObjectMutator& ObjectMutator::add()
{
	if (m_Obj.get<Component>())
	{
		assert(false && "component already exists!");
	}
	else
	{
		m_Obj.components[TypeIdGenerator::Get<Component>()] = std::make_shared<Component>();
		GetSingletonInstance<ComponentDependancyRules>().applyDependancies<Component>(*this);
	}
	return *this;
}

template<typename Component>
inline ObjectMutator& ObjectMutator::add(const std::shared_ptr<Component>& component)
{
	if (m_Obj.get<Component>())
	{
		assert(false && "component already exists!");
	}
	else
	{
		m_Obj.components[TypeIdGenerator::Get<Component>()] = component;
		GetSingletonInstance<ComponentDependancyRules>().applyDependancies<Component>(*this);
	}
	return *this;
}

template<typename Component>
inline ObjectMutator& ObjectMutator::add(std::shared_ptr<Component>&& component)
{
	if (m_Obj.get<Component>())
	{
		assert(false && "component already exists!");
	}
	else
	{
		m_Obj.components[TypeIdGenerator::Get<Component>()] = std::move(component);
		GetSingletonInstance<ComponentDependancyRules>().applyDependancies<Component>(*this);
	}
	return *this;
}

template <typename Component>
ObjectMutator& ObjectMutator::remove()
{
	m_Obj.components.erase(TypeIdGenerator::Get<Component>());
	return *this;
}