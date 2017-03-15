#include "GenericVariable.h"

#include <stdexcept>

using namespace std;

namespace ShaderSystem
{
	GenericVariable::~GenericVariable()
	{
	}

	GenericVariable::GenericVariable() :m_Data(nullptr), m_Type(NONE)
	{
	}

	GenericVariable::GenericVariable(void* ptr, GenericType Type) : m_Data(ptr), m_Type(Type)
	{}

	void GenericVariable::operator=(const GenericVariable& var)
	{
		if (var.m_Type != m_Type)
			throw runtime_error("Different types during assign");
		memcpy(m_Data, var.m_Data, m_Type);
	}

	pair<void*, GenericType> GenericVariable::Get()
	{
		return {m_Data, m_Type};
	}

	void GenericVariable::Set(void* ptr, GenericType type)
	{
		m_Data = ptr;
		m_Type = type;
	}

	void GenericVariable::Set(void* ptr)
	{
		m_Data = ptr;
	}

	GenericType GenericVariable::Type() const
	{
		return m_Type;
	}
}