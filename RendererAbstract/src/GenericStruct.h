#pragma once
#include <vector>
#include <unordered_map>

#include "GenericType.h"
#include "Memory.h"
#include "GenericVariable.h"


namespace ShaderSystem
{
	class GenericStruct
	{
	public:
		GenericStruct();
		explicit GenericStruct(const std::vector<std::pair<std::string, std::string>>& TypesAndNames);
		explicit GenericStruct(const std::vector<std::pair<GenericType, std::string>>& TypesAndNames);
		GenericStruct(const GenericStruct&);
		void operator=(const GenericStruct&);
		GenericStruct(GenericStruct&&);
		void operator=(GenericStruct&&);
		void push(const std::string& Type, const std::string& Name);
		void push(GenericType Type, const std::string& Name);
		GenericVariable& operator[](int index);
		GenericVariable& operator[](const std::string& name);
		const vector<GenericVariable*>& Variables()const;
		~GenericStruct();
	private:
		void Construct(const std::vector<std::pair<GenericType, std::string>>& TypesAndNames);

		std::unordered_map<std::string, GenericVariable> m_VarsByName;
		std::vector<GenericVariable*> m_Vars;
		Memory m_Memory;
	};
}
