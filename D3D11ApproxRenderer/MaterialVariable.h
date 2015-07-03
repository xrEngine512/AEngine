#pragma once
#include <string>
#include "IMaterialVariable.h"
#include "MaterialInterfaceManager.h"

namespace MatInterface
{
	class AbstractMaterialVariable : public IMaterialVariable
	{
	protected:
		int id;
		std::string m_name, description;
		VarGroup m_group;
	public:
		AbstractMaterialVariable(int ID, char* name, VarGroup group, char* desc) : id(ID), m_name(name), description(desc),m_group(group)
		{}
		int ID()const override
		{
			return id;
		}
		const char* Description()const override
		{
			return description.c_str();
		}
		const char* Name()const override
		{
			return m_name.c_str();
		}
		VarGroup Group()const override
		{
			return m_group;
		}

		virtual ~AbstractMaterialVariable(){}
	};

	template <class T>
	class MaterialVariable : public AbstractMaterialVariable
	{
		T value;

	public:
		MaterialVariable(int ID, T &value, char* name, VarGroup group, char* desc = "No Description") : AbstractMaterialVariable(ID, name,group, desc), value(value)
		{
			g_materialInterfaceManager->RegisterVariable(this);
		}

		MaterialVariable(int ID, char* name, VarGroup group, char* desc = "No Description") : AbstractMaterialVariable(ID, name,group, desc), value()
		{
			g_materialInterfaceManager->RegisterVariable(this);
		}

		MaterialVariable(MaterialVariable<T> const& arg) = default;

		void* Ptr() override
		{
			return static_cast<void*>(&value);
		}
		int Size()const override
		{
			return sizeof(T);
		}
		const char* Type()const override
		{
			return typeid(T).name();
		}

		void operator=(MaterialVariable<T> const& arg)
		{
			value = arg.value;
		}

		void operator=(T const& val)
		{
			value = val;
		}

		T Value()const
		{
			return value;
		}

		virtual ~MaterialVariable()
		{

		}
	};
}
