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

		template<class M>
		static M __vectorcall Multiply(const M& arg)
		{
			return arg;
		}

		template<class M, class... Ms>
		static T __vectorcall Multiply(const M& arg, const Ms&... args)
		{
			return arg * Multiply<Ms...>(args...);
		}	
		
		void operator=(MaterialVariable const& arg)
		{
			value = arg.value;
		}			

		T __vectorcall operator*(MaterialVariable const& arg) const
		{
			return value * arg.value;
		}

		T __vectorcall operator*(T const& val) const
		{
			return value * val;
		}

		void operator=(T const& val)
		{
			value = val;
		}

		T __vectorcall Value()const
		{
			return value;
		}

		virtual ~MaterialVariable()
		{

		}
	};
	template<class M>
	inline M Multiply(const M& arg)
	{
		return arg;
	}

	template<class M, class... Ms>
	inline auto __vectorcall Multiply(const M& arg, const Ms&... args) -> decltype(arg * arg)
	{
		return arg * Multiply<Ms...>(args...);
	}
}
