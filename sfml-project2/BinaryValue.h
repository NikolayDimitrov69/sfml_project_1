#pragma once

enum class ValueType 
{ 
	Int, 
	Bool, 
	Float, 
	String, 
	Struct, 
	Array 
};

struct BinaryValue {
    virtual ~BinaryValue() {}
};

struct IntValue : BinaryValue {
    int value;
    IntValue(int v) : value(v) {}
};

struct BoolValue : BinaryValue {
    bool value;
    BoolValue(bool v) : value(v) {}
};

struct FloatValue : BinaryValue {
    float value;
    FloatValue(float v) : value(v) {}
};

struct StringValue : BinaryValue {
    std::string value;
    StringValue(std::string v) : value(std::move(v)) {}
};

struct BinaryStruct : BinaryValue {
    std::unordered_map<std::string, std::shared_ptr<BinaryValue>> fields;

    template<typename T>
    T Get(const std::string& name) const;

    template<typename T>
    bool Get(const std::string& name, T* out) const;
};

struct BinaryStructArray : BinaryValue {
    std::vector<std::shared_ptr<BinaryStruct>> items;

    std::shared_ptr<BinaryStruct> Get(size_t index) const {
        if (index >= items.size()) return nullptr;
        return items[index];
    }

    size_t GetElementCount() const {
        return items.size();
    }

    void Add(std::shared_ptr<BinaryStruct> item) {
        items.push_back(item);
    }
};

template<> inline int BinaryStruct::Get<int>(const std::string& name) const {
    auto val = std::dynamic_pointer_cast<IntValue>(fields.at(name));
    if (!val)
    {
        return 0;
    }
    return val->value;
}
template<> inline bool BinaryStruct::Get<bool>(const std::string& name) const {
    auto val = std::dynamic_pointer_cast<BoolValue>(fields.at(name));
    if (!val)
    {
        return false;
    }
    return val->value;
}
template<> inline float BinaryStruct::Get<float>(const std::string& name) const {
    auto val = std::dynamic_pointer_cast<FloatValue>(fields.at(name));
    if (!val)
    {
        return 0.0;
    }
    return val->value;
}
template<> inline std::string BinaryStruct::Get<std::string>(const std::string& name) const {
    auto val = std::dynamic_pointer_cast<StringValue>(fields.at(name));
    if (!val)
    {
        return "";
    }
    return val->value;
}
template<> inline BinaryStruct* BinaryStruct::Get<BinaryStruct*>(const std::string& name) const {
    auto val = std::dynamic_pointer_cast<BinaryStruct>(fields.at(name));
    if (!val)
    {
        return nullptr;
    }
    return val.get();
}
template<> inline BinaryStructArray* BinaryStruct::Get<BinaryStructArray*>(const std::string& name) const {
    auto val = std::dynamic_pointer_cast<BinaryStructArray>(fields.at(name));
    if (!val)
    {
        return nullptr;
    }
    return val.get();
}
template<> inline bool BinaryStruct::Get<int>(const std::string& name, int* out) const {
    auto val = std::dynamic_pointer_cast<IntValue>(fields.at(name));
    if (!val)
    {
        return false;
    }
    *out = val->value;
    return true;
}
template<> inline bool BinaryStruct::Get<bool>(const std::string& name, bool* out) const {
    auto val = std::dynamic_pointer_cast<BoolValue>(fields.at(name));
    if (!val)
    {
        return false;
    }
    *out = val->value;
    return true;
}
template<> inline bool BinaryStruct::Get<float>(const std::string& name, float* out) const {
    auto val = std::dynamic_pointer_cast<FloatValue>(fields.at(name));
    if (!val)
    {
        return false;
    }
    *out = val->value;
    return true;
}
template<> inline bool BinaryStruct::Get<std::string>(const std::string& name, std::string* out) const {
    auto val = std::dynamic_pointer_cast<StringValue>(fields.at(name));
    if (!val)
    {
        return false;
    }
    *out = val->value;
    return true;
}
template<> inline bool BinaryStruct::Get<BinaryStruct>(const std::string& name, BinaryStruct* out) const {
    auto val = std::dynamic_pointer_cast<BinaryStruct>(fields.at(name));
    if (!val)
    {
        return false;
    }
    out = val.get();
    return true;
}
template<> inline bool BinaryStruct::Get<BinaryStructArray>(const std::string& name, BinaryStructArray* out) const {
    auto val = std::dynamic_pointer_cast<BinaryStructArray>(fields.at(name));
    if (!val)
    {
        return false;
    }
    out = val.get();
    return true;
}