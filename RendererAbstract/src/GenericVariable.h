#pragma once

#include "GenericType.h"
#include "ShaderSystemTypes.h"

#include <cstring>
#include <assert.h>


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
			assert(sizeof(T) < m_Type && "Different sizes during assign");
			if (m_Data)
				memcpy(m_Data, &val, m_Type);
		}

		std::pair<void*, GenericType> Get();
		void Set(void* ptr, GenericType type);
		void Set(void* ptr);
		GenericType Type()const;
	private:
		void* m_Data;
		GenericType m_Type;

	};
}
