#pragma once
#include <utility>
#include <exception>
#include "GenericType.h"

using std::pair;

namespace ShaderSystem
{
	struct GenericVariable
	{
		GenericVariable();
		GenericVariable(void* ptr, GenericType Type);
		~GenericVariable();

		void operator=(const GenericVariable& var);

		template<typename T>
		void operator=(const T& val)
		{
			if (sizeof(T) < m_Type)
				throw std::exception("Different sizes during assign");
			memcpy(m_Data, &val, m_Type);
		}

		pair<void*, GenericType> Get();
		GenericType Type()const;
	private:
		void* m_Data;
		GenericType m_Type;

	};
}
