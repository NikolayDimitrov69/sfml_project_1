#pragma once

class GameObjectsLoader
{
public:
	void Load();
	
	DECLARE_SINGLETON(GameObjectsLoader);
private:
	void LoadStringTable();
	std::string ReadStringFromTable(uint32_t idx);
	void LoadObjectsFromMemory();
	bool FillBuffer();
	void InitializeGameObjects();

	std::shared_ptr<BinaryStruct> DeserializeStruct(size_t& pos);
	std::shared_ptr<BinaryStructArray> DeserializeStructArray(size_t& pos);
	std::shared_ptr<BinaryValue> DeserializeValue(size_t& pos, ValueType type);	

	template<typename T>
	T Read(size_t& pos)
	{
		T val{};
		if (pos + sizeof(T) > m_Buffer.size()) return val;
		std::memcpy(&val, &m_Buffer[pos], sizeof(T));
		pos += sizeof(T);
		return val;
	}
private:
	std::vector<std::string> m_StrTable;
	std::vector<uint8_t> m_Buffer;
	std::vector<std::shared_ptr<BinaryStruct>> m_Objects;
};

