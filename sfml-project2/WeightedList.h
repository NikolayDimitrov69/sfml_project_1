#pragma once

template <typename Item>
class WeightedList
{
public:
	WeightedList() 
		: m_TotalWeight(0)
	{}

	bool Empty() const
	{
		return m_InternalList.empty();
	}

	void Clear()
	{
		m_InternalList.clear();
	}

	void Add(const Item& item, int weight)
	{
		AddInternal(item, weight);
	}

	bool AddUnique(const Item& item, int weight)
	{
		ReturnIf(m_InternalList.find(item) != m_InternalList.end(), false);
		AddInternal(item, weight);
		return true;
	}

	Item GetRandomItem() const
	{
		int r = RandomNumber(0, m_TotalWeight - 1);
		for (const auto& elem : m_InternalList)
		{
			if (r < elem.second)
			{
				return elem.first;
			}
			r -= elem.second;
		}

		ReturnIf(true, Item{});
	}
private:

	void AddInternal(const Item& item, int weight)
	{
		m_TotalWeight += weight;
		auto itemIt = m_InternalList.find(item);
		if (itemIt != m_InternalList.end())
		{
			itemIt->second += weight;
		}
		else
		{
			m_InternalList.emplace(item, weight);
		}
	}
private:
	std::map<Item, int> m_InternalList;
	int	m_TotalWeight;
};

