#pragma once
#include <unordered_map>
#include <memory>

using std::unordered_map;
using std::shared_ptr;

template<class ValType, class KeyType, class Container = unordered_map<KeyType, shared_ptr<ValType>>>
class GenericPool
{
protected:
	Container m_Cache;
	typedef Container::value_type ContainerElement;
public:
	typedef shared_ptr<ValType> ValPtr;
	GenericPool()
	{
		
	}

	ValPtr GetCreate(const KeyType& key)
	{
		auto res = m_Cache.find(key);
		if (res != m_Cache.end())
		{
			return res->second;
		}
		auto newElement = std::make_shared<ValType>();
		m_Cache.insert(ContainerElement(key, newElement));
		return newElement;
	}

	ValPtr Get(const KeyType& key)const
	{
		auto res = m_Cache.find(key);
		if (res != m_Cache.end())
		{
			return res->second;
		}
		return nullptr;		
	}

	virtual ~GenericPool()
	{
	}
};

