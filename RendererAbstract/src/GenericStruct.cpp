#include <ApproxSystemErrors.h>
#include "GenericStruct.h"
#include "GenericVariable.h"
#include "MemoryBlock.h"
#include "ShaderSystemTypes.h"

using namespace std;

namespace ShaderSystem
{
	GenericStruct::GenericStruct() :m_Memory()
	{
	}

	GenericStruct::GenericStruct(const vector<pair<string, string>>& TypesAndNames) : GenericStruct()
	{
		vector<pair<GenericType, string>> Info;
		Info.reserve(TypesAndNames.size());
		for (auto entry : TypesAndNames)
		{
			Info.push_back(pair<GenericType, string>(ReadType(entry.first), entry.second));
		}
		Construct(Info);
	}

	GenericStruct::GenericStruct(const vector<pair<GenericType, string>>& TypesAndNames) : GenericStruct()
	{
		Construct(TypesAndNames);
	}

	GenericStruct::GenericStruct(const GenericStruct& arg)
	{
		if (this != &arg)
		{
			m_VarsByName = arg.m_VarsByName;
			m_Memory = arg.m_Memory;
			int i = 0;
			for (auto& var : m_VarsByName)
			{
				var.second.Set(m_Memory[i++].Data());
				m_Vars.push_back(&var.second);
			}
		}
	}

	void GenericStruct::operator=(const GenericStruct& arg)
	{
		if (this != &arg)
		{
			m_VarsByName = arg.m_VarsByName;
			m_Memory = arg.m_Memory;
			int i = 0;
			m_Vars.clear();
			for (auto& var : m_VarsByName)
			{
				var.second.Set(m_Memory[i++].Data());
				m_Vars.push_back(&var.second);
			}
		}
	}

	inline void GenericStruct::push(const string& Type, const string& Name)
	{
		push(ReadType(Type), Name);
	}

	void GenericStruct::push(GenericType Type, const string& Name)
	{
		m_VarsByName.insert({Name, GenericVariable(m_Memory.addBlock(Type).Data(), Type)});
		m_Vars.push_back(&m_VarsByName.find(Name)->second);
	}

	GenericVariable& GenericStruct::operator[](int index)
	{
		if (index < m_Vars.size())
			return *m_Vars[index];
		throw approx_exception("GenericStruct: index out of range");
	}

	GenericVariable& GenericStruct::operator[](const string& name)
	{
		auto res = m_VarsByName.find(name);
		if (res != m_VarsByName.end())
			return res->second;
        else {
            auto var = new GenericVariable();
            m_Vars.push_back(var);
            m_VarsByName[name] = var;
            return *var;
        }
	}

	const vector<GenericVariable*>& GenericStruct::Variables() const
	{
		return m_Vars;
	}

	GenericStruct::~GenericStruct()
	{
	}

	inline void GenericStruct::Construct(const vector<pair<GenericType, string>>& TypesAndNames)
	{
		size_t size = 0;
		for (auto entry : TypesAndNames)
		{
			size += static_cast<size_t>(entry.first);
		}
		m_Memory.setSize(size);
		for (auto entry : TypesAndNames)
		{
			push(entry.first, entry.second);
		}
	}

	GenericStruct::GenericStruct(GenericStruct && lhr):
		m_VarsByName(move(lhr.m_VarsByName)),
		m_Vars(move(lhr.m_Vars)),
		m_Memory(move(lhr.m_Memory))
	{

	}

	void GenericStruct::operator=(GenericStruct && lhr) {
		if(&lhr != this) {
			m_VarsByName = move(lhr.m_VarsByName);
			m_Vars = move(lhr.m_Vars);
			m_Memory = move(lhr.m_Memory);
		}
	}
}