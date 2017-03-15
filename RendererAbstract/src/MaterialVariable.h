#pragma once
#include "IMaterialVariable.h"
#include "MaterialInterfaceManager.h"

#include <string>
#include <typeinfo>


namespace MatInterface
{
	class AbstractMaterialVariable : public IMaterialVariable
	{
	protected:
		int id;
		std::string m_name, description;
		VarGroup m_group;
	public:
		AbstractMaterialVariable(int ID, const std::string& name, VarGroup group, const std::string& desc) : id(ID), m_name(name), description(desc),m_group(group)
		{}
		int ID()const override
		{
			return id;
		}
		const std::string& Description()const override
		{
			return description;
		}
		const std::string& Name()const override
		{
			return m_name;
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
		MaterialVariable(int ID, T &value, const std::string& name, VarGroup group, const std::string& desc = "No Description") : AbstractMaterialVariable(ID, name,group, desc), value(value)
		{
			g_materialInterfaceManager->RegisterVariable(this);
		}

		MaterialVariable(int ID, const std::string& name, VarGroup group, const std::string& desc = "No Description") : AbstractMaterialVariable(ID, name,group, desc), value()
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
		std::string type()const noexcept override
		{
			return typeid(T).name();
		}
		
		void operator=(MaterialVariable const& arg)
		{
			value = arg.value;
		}

		T operator=(T const& val)
		{
			return value = val;
		}

        template <class _T>
        friend _T operator*(const MaterialVariable<_T>& lhs, const MaterialVariable<_T>& rhs);

        template <class _T>
        friend _T operator*(const MaterialVariable<_T>& lhs, const _T& rhs);

        template <class _T>
        friend _T operator*(const _T& lhs, const MaterialVariable<_T>& rhs);

        operator T() {
            return value;
        }

        operator T() const {
            return value;
        }

		T simdcall Value()const
		{
			return value;
		}

		virtual ~MaterialVariable()
		{

		}
	};

    template <class T>
    T operator*(const MaterialVariable<T>& lhs, const MaterialVariable<T>& rhs) {
        return lhs.value * rhs.value;
    }

    template <class T>
    T operator*(const MaterialVariable<T>& lhs, const T& rhs) {
        return lhs.value * rhs;
    }

    template <class T>
    T operator*(const T& lhs, const MaterialVariable<T>& rhs) {
        return lhs * rhs.value;
    }
}
