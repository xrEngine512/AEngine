#pragma once
#include <vector>
#include <unordered_map>

#include "GenericType.h"
#include "Memory.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::pair;

namespace ShaderSystem
{
	class GenericVariable;

	class GenericStruct
	{
	public:
		GenericStruct();
		explicit GenericStruct(const vector<pair<string, string>>& TypesAndNames);
		explicit GenericStruct(const vector<pair<GenericType, string>>& TypesAndNames);
		void push(const string& Type, const string& Name);
		void push(GenericType Type, const string& Name);
		GenericVariable& operator[](int index);
		GenericVariable& operator[](const string& name);
		const vector<GenericVariable*>& Variables()const;
		~GenericStruct();
	private:
		void Construct(const vector<pair<GenericType, string>>& TypesAndNames);

		unordered_map<string, GenericVariable> m_VarsByName;
		vector<GenericVariable*> m_Vars;
		Memory m_Memory;
	};
}
