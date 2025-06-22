#include "precompheaders.h"
#include "GameObjectsLoader.h"

void GameObjectsLoader::Load()
{
    ReturnUnless(FillBuffer());
    LoadStringTable();
    LoadObjectsFromMemory();
    InitializeGameObjects();
}

bool GameObjectsLoader::FillBuffer()
{
    std::ifstream in(GAME_OBJECTS_BIN_PATH, std::ios::binary);
    ReturnUnless(in, false);

    m_Buffer = std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return true;
}

void GameObjectsLoader::InitializeGameObjects()
{
    for (const auto& object : m_Objects)
    {
        const auto& components = object->Get<BinaryStructArray*>("components");

        // todo
        GameObject* gameObject = new GameObject();
        ObjectMutator mut(gameObject);

        for (const auto& component : components->items)
        {
            const auto& compName = component->Get<std::string>("name");
            GetSingletonInstance<ComponentFactory>().Create(compName, mut, *component.get());
        }
    }
}

void GameObjectsLoader::LoadStringTable()
{
    for (size_t i = m_Buffer.size() - 4; i > 0; --i) {
        if (i + 4 > m_Buffer.size())
        {
            continue;
        }

        uint32_t count = 0;
        std::memcpy(&count, &m_Buffer[i], sizeof(uint32_t));

        // Plausible string count range (e.g., 1 to 1000)
        if (count == 0 || count > 1000)
        {
            continue;
        }

        size_t pos = i + 4;
        std::vector<std::string> temp;

        bool fail = false;

        for (uint32_t j = 0; j < count; ++j) {
            if (pos + 4 > m_Buffer.size())
            {
                fail = true;
                break;
            }
            uint32_t len = Read<uint32_t>(pos);
            if (pos + len > m_Buffer.size())
            {
                fail = true;
                break;
            }
            std::string s(reinterpret_cast<const char*>(&m_Buffer[pos]), len);
            pos += len;
            temp.push_back(s);
        }

        if (fail)
        {
            continue;
        }

        //only accept if we matched the end
        if (pos == m_Buffer.size()) {
            std::cout << "Found string table at offset " << i << " with " << count << " entries.\n";
            m_StrTable = std::move(temp);
        }
    }
}

std::string GameObjectsLoader::ReadStringFromTable(uint32_t idx)
{
    if (idx >= m_StrTable.size()) return "";
    return m_StrTable[idx];
}

std::shared_ptr<BinaryStruct> GameObjectsLoader::DeserializeStruct(size_t& pos)
{
    uint32_t propCount = Read<uint32_t>(pos);
    auto s = std::make_shared<BinaryStruct>();
    for (uint32_t i = 0; i < propCount; ++i) {
        uint32_t nameIdx = Read<uint32_t>(pos);
        std::string name = ReadStringFromTable(nameIdx);
        ValueType type = static_cast<ValueType>(Read<uint8_t>(pos));
        s->fields[name] = DeserializeValue(pos, type);
    }
    return s;
}

std::shared_ptr<BinaryStructArray> GameObjectsLoader::DeserializeStructArray(size_t& pos)
{
    auto arr = std::make_shared<BinaryStructArray>();
    uint32_t count = Read<uint32_t>(pos);
    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t nameIdx = Read<uint32_t>(pos);
        std::string name = ReadStringFromTable(nameIdx);
        ValueType type = static_cast<ValueType>(Read<uint8_t>(pos));
        if (type == ValueType::Struct)
        {
            arr->Add(DeserializeStruct(pos));
        }
        else
        {
            auto s = std::make_shared<BinaryStruct>();
            s->fields[name] = DeserializeValue(pos, type);
            arr->Add(s);
        }

    }
    return arr;
}

std::shared_ptr<BinaryValue> GameObjectsLoader::DeserializeValue(size_t& pos, ValueType type)
{
    switch (type) {
    case ValueType::Int: return std::make_shared<IntValue>(Read<int>(pos));
    case ValueType::Bool: return std::make_shared<BoolValue>(Read<uint8_t>(pos) != 0);
    case ValueType::Float: return std::make_shared<FloatValue>(Read<float>(pos));
    case ValueType::String: {
        uint32_t idx = Read<uint32_t>(pos);
        return std::make_shared<StringValue>(ReadStringFromTable(idx));
    }
    case ValueType::Struct: return DeserializeStruct(pos);
    case ValueType::Array: return DeserializeStructArray(pos);
    default:
        return nullptr;
    }
}

void GameObjectsLoader::LoadObjectsFromMemory()
{
    size_t pos = 0;
    uint32_t objectCount = Read<uint32_t>(pos);

    for (uint32_t i = 0; i < objectCount; ++i) {
        uint32_t objNameIdx = Read<uint32_t>(pos);
        uint32_t compCount = Read<uint32_t>(pos);

        auto obj = std::make_shared<BinaryStruct>();
        obj->fields["name"] = std::make_shared<StringValue>(ReadStringFromTable(objNameIdx));

        auto comps = std::make_shared<BinaryStructArray>();
        for (uint32_t j = 0; j < compCount; ++j) {
            uint32_t compNameIdx = Read<uint32_t>(pos);
            auto comp = DeserializeStruct(pos);
            comp->fields["name"] = std::make_shared<StringValue>(ReadStringFromTable(compNameIdx));
            comps->Add(comp);
        }

        obj->fields["components"] = comps;
        m_Objects.push_back(obj);
    }
}
