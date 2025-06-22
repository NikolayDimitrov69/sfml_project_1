#pragma once
struct BinaryStruct;

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void Update(const BinaryStruct& data) = 0;
};

class ComponentFactory
{
public:
    using CreatorFunc = std::function<void(ObjectMutator&, const BinaryStruct& data)>;

    template <typename Component>
    void Register(const std::string& name)
    {
        creators[name] = [](ObjectMutator& mutator, const BinaryStruct& data) {
            auto compPtr = std::make_shared<Component>();
            compPtr->Update(data);
            mutator.add<Component>(std::move(compPtr));
            };
    }

    bool Create(const std::string& name, ObjectMutator& mutator, const BinaryStruct& data) const
    {
        auto it = creators.find(name);
        if (it != creators.end())
        {
            it->second(mutator, data);
            return true;
        }
        return false;
    }

    DECLARE_SINGLETON(ComponentFactory);
private:
    std::unordered_map<std::string, CreatorFunc> creators;
};

template<typename T>
class StaticComponentRegistration {
public:
    StaticComponentRegistration(const std::string& compName) {
        static_assert(std::is_base_of<IComponent, T>::value);
        GetSingletonInstance<ComponentFactory>().Register<T>(compName);
    }
};

#define REGISTER_COMPONENT(Component, ComponentName) \
    static StaticComponentRegistration<Component> _static_reg_##Component(ComponentName)
