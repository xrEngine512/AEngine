#pragma once
#include <stdio.h>
#include <string>
#include <d3d11.h>
#include <vector>
#include <ApproxMemory.h>
#include <ApproxSystemErrors.h>
#include <unordered_set>
#include "Literals.h"
#include "AbstractSaveData.h"

enum ShaderPackElementID : unsigned __int16
{
    ID_NONE, COMPILED_VS, COMPILED_PS, COMPILED_GS, COMPILED_CS, COMPILED_DS, COMPILED_HS,
	SAMPLER_DESC, INPUT_LAYOUT_ELEMENT, IL_SEMANTIC_NAME, SHADER_NAME, RUNTIME_BUFFER_INFO,
	TEXTURE_DESC, PARAM_DESC, SHADER_SETS
};

enum class ProjectPackElementID : unsigned __int16
{
	NONE, VS, PS, GS, CS, DS, HS, ENTRY_POINT, SHADER_MODEL, SHADER_NAME,
	APPROX_VAR_BUFFER_INFO, PARAMETER, TEXTURE
};

using std::vector;

struct D3D11_INPUT_LAYOUT_ELEMENT
{
	UINT SemanticIndex;
	DXGI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
	D3D11_INPUT_ELEMENT_DESC ToElemDesc()
	{
		D3D11_INPUT_ELEMENT_DESC res;
		res.AlignedByteOffset = AlignedByteOffset;
		res.Format = Format;
		res.InputSlot = InputSlot;
		res.InputSlotClass = InputSlotClass;
		res.InstanceDataStepRate = InstanceDataStepRate;
		res.SemanticIndex = SemanticIndex;
		res.SemanticName = nullptr;
		return res;
	}
};

template<class DataOwnershipPolicy, class ID_Type>
struct PackElement : DataOwnershipPolicy
{
	struct MetaData
	{
		ID_Type ID;
		__int64 size = 0;
		__int8  numOfChildren = 0;
		MetaData() :ID(static_cast<ID_Type>(0)) {};
	}m_MetaData;
	
	void* Data = nullptr;
	

	PackElement() :m_MetaData()
    {
    }
	PackElement(FILE* &file)
	{
		fread_s(&m_MetaData, sizeof m_MetaData, sizeof m_MetaData, 1, file);
		Data = malloc(m_MetaData.size);
		fread_s(Data, m_MetaData.size, m_MetaData.size, 1, file);
		for (int i = 0; i < m_MetaData.numOfChildren; i++)
		{
			m_children.push_back(new PackElement(file));
		}
	}
	static PackElement* fromFile(FILE* &file)
    {		
		return new PackElement(file);
    }
	PackElement(PackElement const& arg) :m_MetaData(arg.m_MetaData)
	{
		if (this != &arg)
		{
			Copy(Data, arg.m_MetaData.size, arg.Data);
			DestroyChildren();
			m_allowedDuplicates = arg.m_allowedDuplicates;
			affectHierarchy = arg.affectHierarchy;
			for (auto child : arg.m_children)
			{
				*this << child;
			}
		}
	}
	static PackElement* fromMemory(ID_Type _ID, size_t _size, const void* pData)
	{
		auto res = new PackElement;
		Copy(res->Data, _size, pData);
		res->m_MetaData.ID = _ID;
		res->m_MetaData.size = _size;
		return res;
	}

	template<typename T>
	static PackElement* fromObj(ID_Type _ID, const T& obj)
	{
		auto res = new PackElement;
		res->m_MetaData.ID = _ID;
		res->m_MetaData.size = sizeof(T);
		Copy(res->Data, obj);
		return res;
	}
	
	static PackElement* fromC_Str(ID_Type _ID, const char* c_str)
	{
		auto res = new PackElement;
		res->m_MetaData.ID = _ID;
		res->m_MetaData.size = strlen(c_str) + 1;
		Copy(res->Data, res->m_MetaData.size, c_str);
		return res;
	}
	static PackElement* fromStdString(ID_Type _ID, std::string& str)
	{
		auto res = new PackElement;
		res->m_MetaData.ID = _ID;
		res->m_MetaData.size = str.size() + 1;
		Copy(res->Data, res->m_MetaData.size, str.c_str());
		return res;
	}
	
	static PackElement* fromSaveData(ID_Type _ID, ASL::AbstractSaveData* obj)
	{
		auto res = new PackElement;
		int size;
		auto buf = obj->Serialize(size);
		Copy(res->Data, size, buf);
		res->m_MetaData.ID = _ID;
		res->m_MetaData.size = size;
		obj->CleanSerializedBuffer();
		return res;
	}

	template<typename T>
	static typename std::enable_if<std::is_base_of<ASL::AbstractSaveData, T>::value, PackElement*>::type fromSaveData(ID_Type _ID, T& obj)
	{
		return fromSaveData(_ID, dynamic_cast<ASL::AbstractSaveData*>(&obj));
	}

	void operator=(PackElement const& arg)
	{
		if (this != &arg)
		{
			Copy(m_MetaData, arg.m_MetaData);
			Copy(Data, arg.m_MetaData.size, arg.Data);
			m_allowedDuplicates = arg.m_allowedDuplicates;
			affectHierarchy = arg.affectHierarchy;
			DestroyChildren();
			for (auto child : arg.m_children)
			{
				m_children.push_back(new PackElement(*child));
				++m_MetaData.numOfChildren;
			}
		}
	}
	PackElement* Child(int index)
	{
		if (index < m_children.size())
		{
			return m_children[index];
		}
		return nullptr;
	}
	template <typename T>
	typename std::enable_if<!std::is_base_of<ASL::AbstractSaveData, T>::value>::type Get(T& obj) const
	{
		if (m_MetaData.size != sizeof(T))
			throw std::exception("Sizes mismatch!");

		memcpy(&obj, Data, m_MetaData.size);
		return;
	}
	template <typename T>
	typename std::enable_if<std::is_base_of<ASL::AbstractSaveData, T>::value>::type Get(T& obj) const
	{
		return obj.Deserialize(Data, m_MetaData.size);
	}

	void Get (std::string& obj)const
	{
		obj = static_cast<const char*>(Data);
		return;
	}
	void Get (std::wstring& obj)const
	{
		obj = static_cast<const wchar_t*>(Data);
		return;
	}
	template <class T>
	size_t Get(T*& ptr)const
	{
		ptr = static_cast<T*>(malloc(m_MetaData.size));
		memcpy((void*)ptr, Data, m_MetaData.size);
		return m_MetaData.size;
	}
	void Get(ASL::AbstractSaveData* obj)const
	{
		obj->Deserialize(Data, m_MetaData.size);
	}
	void AllowDuplicatesFor(ID_Type ID, bool affectAllHierarchy = false)
	{
		m_allowedDuplicates.insert(ID);
		if (affectAllHierarchy)
		{
			for (auto child : m_children)
			{
				child->AllowDuplicatesFor(ID, affectAllHierarchy);
			}
		}
	}
	void AllowDuplicatesFor(const std::unordered_set<ID_Type>& IDs, bool affectAllHierarchy = false)
	{
		m_allowedDuplicates = IDs;
		if (affectAllHierarchy)
		{
			for (auto child : m_children)
			{
				child->AllowDuplicatesFor(IDs, affectAllHierarchy);
			}
		}
	}
	PackElement* Find(ID_Type ID) const
	{
		for (auto child : m_children)
		{
			if (child->m_MetaData.ID == ID)
			{
				return child;
			}
		}
		return nullptr;
	}
	std::vector<PackElement*> FindMany(ID_Type ID) const
	{
		std::vector<PackElement*> res;
		for (auto child : m_children)
		{
			if (child->m_MetaData.ID == ID)
			{
				res.push_back(child);
			}
		}
		return res;
	}
	void operator>>(FILE* file)
	{
		//Write data to disk
		fwrite(&m_MetaData, sizeof(m_MetaData), size_t(1), file);
		fwrite(Data, m_MetaData.size, size_t(1), file);

		//Write children data to disk
		for (auto child : m_children)
		{
			*child >> file;
		}
	}
	void operator<<(PackElement* _child)
	{
		if (m_allowedDuplicates.find(_child->m_MetaData.ID) == m_allowedDuplicates.end())
			for (auto child : m_children)
			{
				if (child->m_MetaData.ID == _child->m_MetaData.ID)
				{
					delete child;
					child = _child;
					child->m_MetaData.ID = _child->m_MetaData.ID;
					return;
				}
			}
		if (affectHierarchy)
			_child->AllowDuplicatesFor(m_allowedDuplicates, true);

		m_children.push_back(_child);
		++m_MetaData.numOfChildren;
	}
	
	void DestroyChildren()
	{
		for (auto child : m_children)
		{
			delete child;
			child = nullptr;
		}
		m_children.clear();
		m_MetaData.numOfChildren = 0;
	}
	~PackElement()
	{
		Delete(Data);
		DestroyChildren();
	}
private:
	std::vector<PackElement*> m_children;
	bool affectHierarchy = false;
	std::unordered_set<ID_Type> m_allowedDuplicates;
};

template<class DataOwnershipPolicy, class ID_Type>
class PackFile
{
public:
	typedef PackElement<DataOwnershipPolicy, ID_Type> LocalElement;
protected:
	const char * m_fileExtention;
	bool affectHierarchy = false;
	FILE* m_file;
	std::unordered_set<ID_Type> m_allowedDuplicates;
	std::vector<LocalElement*> m_Elements;

public:
	PackFile() :m_file(nullptr)
	{}
	PackFile(const char* fileName, bool addExtention = true)
	{
		OpenFile(fileName, addExtention);
	}
	PackFile(const wchar_t* fileName, bool addExtention = true)
	{
		OpenFile(fileName, addExtention);
	}
	void operator()(const char* fileName, bool addExtention = true)
	{ 
		if (m_file) 
			throw ApproxException(FILE_ALREADY_OPENED, ApproxException::INSTANT_MSG_DISPLAY); 
		OpenFile(fileName, addExtention);
	}
	void operator()(const wchar_t* fileName, bool addExtention = true)
	{ 
		if (m_file) 
			throw ApproxException(FILE_ALREADY_OPENED, ApproxException::INSTANT_MSG_DISPLAY);
		OpenFile(fileName, addExtention);
	}
	virtual void ToDisk()
	{
		for (auto element : m_Elements)
		{
			*element >> m_file;
		}
	}
	void AllowDuplicatesFor(ID_Type ID, bool affectAllHierarchy = false)
	{
		affectHierarchy = affectAllHierarchy;
		m_allowedDuplicates.insert(ID);
		if (affectAllHierarchy)
		{
			for (auto element : m_Elements)
			{
				element->AllowDuplicatesFor(ID, affectAllHierarchy);
			}
		}
	}
	virtual void FromDisk()
	{
		ClearElements();
		while (feof(m_file) == 0)
		{
			*this << new LocalElement(m_file);
		}
	}

	LocalElement* Find(ID_Type ID)
	{
		for (auto element : m_Elements)
		{
			if (element->m_MetaData.ID == ID)
			{
				return element;
			}
		}
		return nullptr;
	}
	vector<LocalElement*> FindMany(ID_Type ID)
	{
		vector<LocalElement*> res;
		for (auto elem : m_Elements)
		{
			if (elem->m_MetaData.ID == ID)
			{
				res.push_back(elem);
			}
		}
		return res;
	}
	void operator<<(LocalElement *elem)
	{
		if (m_allowedDuplicates.find(elem->m_MetaData.ID) == m_allowedDuplicates.end())
			for (int i = 0; i < m_Elements.size(); i++)
			{
				auto& element = m_Elements[i];
				if (element->m_MetaData.ID == elem->m_MetaData.ID)
				{
					delete element;
					element = elem;
					return;
				}
			}
		if (affectHierarchy)
			elem->AllowDuplicatesFor(m_allowedDuplicates, affectHierarchy);
		m_Elements.push_back(elem);
	}
	bool operator>>(LocalElement*& OutElem)
	{
		if (counter < m_Elements.size())
		{
			OutElem = m_Elements[counter++];
			return true;
		}
		OutElem = nullptr;
		counter = 0;
		return false;

	}
	void Reset()
	{
		ClearElements();
		counter = 0;
	}
	void CloseFile()
	{
		if (m_file)
		{
			fclose(m_file);
			m_file = nullptr;
		}
	}
	void ClearElements()
	{
		for (auto element : m_Elements)
		{
			delete element;
		}
		m_Elements.clear();
	}
	~PackFile()
	{
		ClearElements();
		CloseFile();
	}
private:
	int counter = 0;
	void OpenFile(const char* fileName, bool write)
	{
		if (write)
		{
			std::string temp(fileName);
			temp += m_fileExtention;
			fopen_s(&m_file, temp.c_str(), "wb");
		}
		else
		{
			fopen_s(&m_file, fileName, "rb");
		}
		if (m_file == nullptr)
			throw ApproxException(OPEN_FILE_FAIL);
	}
	void OpenFile(const wchar_t* fileName, bool write)
	{
		if (write)
		{
			std::wstring temp(fileName);
			size_t len = strlen(m_fileExtention);
			const wchar_t* ext = new wchar_t[len];
			mbstowcs(ext, m_fileExtention, len);
			temp += ext;
			delete[] ext;
			_wfopen_s(&m_file, temp.c_str(), L"wb");
		}
		else
		{
			_wfopen_s(&m_file, fileName, L"rb");
		}
		if (m_file == nullptr)
			throw ApproxException(OPEN_FILE_FAIL);
	}
};

template<class DataOwnershipPolicy>
class ShaderPackFile : public PackFile<DataOwnershipPolicy, ShaderPackElementID>
{
public:
	ShaderPackFile(){ m_fileExtention = ".acs"; }
	/*void ToDisk()override final
    {
		for (auto element : m_Elements)
        {
            if (element->m_MetaData.ID == INPUT_LAYOUT)
            {
                int numOfElems = element->m_MetaData.size / sizeof(D3D11_INPUT_ELEMENT_DESC);
                for (int i = 0; i < numOfElems; i++)
                {
                    LocalElement subStrElement(IL_SEMANTIC_NAME, reinterpret_cast<D3D11_INPUT_ELEMENT_DESC*>(element->Data)[i].SemanticName);
                    delete[] reinterpret_cast<D3D11_INPUT_ELEMENT_DESC*>(element->Data)[i].SemanticName;
                    reinterpret_cast<D3D11_INPUT_ELEMENT_DESC*>(element->Data)[i].SemanticName = nullptr;
                    subStrElement >> m_file;
                }
            }
			*element >> m_file;
        }
    }*/
};

template<class DataOwnershipPolicy>
class ProjectPackFile : public PackFile < DataOwnershipPolicy, ProjectPackElementID >
{
public:
	ProjectPackFile(){ m_fileExtention = ".asp"; }
};