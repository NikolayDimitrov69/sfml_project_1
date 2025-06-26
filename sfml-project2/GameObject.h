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

using DependancyFunction = std::function<void(ObjectMutator&)>;

class ObjectMutator
{
public:
	ObjectMutator(GameObject* obj);

	~ObjectMutator();

	void reset();

	void apply();

	template <typename Component>
	ObjectMutator& add();

	template <typename Component>
	ObjectMutator& add(const std::shared_ptr<Component>& component);

	template <typename Component>
	ObjectMutator& add(std::shared_ptr<Component>&& component);

	template <typename Component>
	ObjectMutator& remove();

private:
	template <typename Component>
	void appendDependancies();

	void applyStoredDependancies();
private:
	GameObject& m_Obj;
	GameObject* m_Mutation;
	std::vector<DependancyFunction> m_MutationDependancies;
};

class ComponentDependancyRules
{
public:
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

	template <typename Dependee>
	const std::vector<DependancyFunction>& getDependancies() const
	{
		static std::vector<DependancyFunction> s_EmptyDependacyRules;
		auto it = dependanciesMap.find(TypeIdGenerator::Get<Dependee>());
		if (it != dependanciesMap.end())
		{
			return it->second;
		}
		return s_EmptyDependacyRules;
	}
	
	DECLARE_SINGLETON(ComponentDependancyRules);
private:
	std::unordered_map<std::size_t, std::vector<DependancyFunction>> dependanciesMap;
};

template <typename Component>
inline void ObjectMutator::appendDependancies()
{
	const auto& dep = GetSingletonInstance<ComponentDependancyRules>().getDependancies<Component>();
	m_MutationDependancies.insert(std::end(m_MutationDependancies), std::begin(dep), std::end(dep));
}

template <typename Component>
inline ObjectMutator& ObjectMutator::add()
{
	if (m_Obj.get<Component>() || m_Mutation->get<Component>())
	{
		assert(false && "component already exists!");
	}
	else
	{
		m_Mutation->components[TypeIdGenerator::Get<Component>()] = std::make_shared<Component>();
		appendDependancies<Component>();
	}
	return *this;
}

template<typename Component>
inline ObjectMutator& ObjectMutator::add(const std::shared_ptr<Component>& component)
{
	if (m_Obj.get<Component>() || m_Mutation->get<Component>())
	{
		assert(false && "component already exists!");
	}
	else
	{
		m_Mutation->components[TypeIdGenerator::Get<Component>()] = component;
		appendDependancies<Component>();
	}
	return *this;
}

template<typename Component>
inline ObjectMutator& ObjectMutator::add(std::shared_ptr<Component>&& component)
{
	if (m_Obj.get<Component>() || m_Mutation->get<Component>())
	{
		assert(false && "component already exists!");
	}
	else
	{
		m_Mutation->components[TypeIdGenerator::Get<Component>()] = std::move(component);
		appendDependancies<Component>();
	}
	return *this;
}

template <typename Component>
ObjectMutator& ObjectMutator::remove()
{
	m_Mutation->components.erase(TypeIdGenerator::Get<Component>());
	return *this;
}