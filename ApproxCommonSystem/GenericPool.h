#pragma once
#include <unordered_map>

using std::unordered_map;

template<class ValType, class KeyType, class Container = unordered_map<KeyType, ValType>>
class GenericPool
{
	Container m_Cache;
public:

	GenericPool()
	{
		
	}

	KeyType Get(const KeyType& key)const
	{
		auto res = m_Cache.find(key);
		if (res != m_Cache.end())
		{
			return res->second;
		}
		
	}

	virtual ~GenericPool()
	{
	}
};

