#pragma once
#include <stdio.h>
#include <string>
#include <d3d11.h>
#include <vector>
#include <ApproxMemory.h>
#include <ApproxSystemErrors.h>
#include "Literals.h"

enum ShaderPackElementID : unsigned __int16
{
    ID_NONE, COMPILED_VS, COMPILED_PS, COMPILED_GS, COMPILED_CS, COMPILED_DS, COMPILED_HS,
	SAMPLER_DESC, INPUT_LAYOUT, IL_SEMANTIC_NAME, SHADER_NAME, CONSTANT_DATA_BUFFER, 
	PER_FRAME_DATA_BUFFER, PER_OBJECT_DATA_BUFFER, SHADER_PARAMS
};

enum class ProjectPackElementID : unsigned __int16
{
	NONE, VS, PS, GS, CS, DS, HS, ENTRY_POINT, SHADER_MODEL, SHADER_NAME,
	APPROX_VAR_BUFFER_INFO
};

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
		MetaData() = default;
	}m_MetaData;

	void* Data = nullptr;
	

	PackElement() :m_MetaData()
    {
    }
    explicit PackElement(FILE* &file)
    {
        fread_s(&m_MetaData, sizeof m_MetaData, sizeof m_MetaData, 1, file);
        Data = malloc(m_MetaData.size);
        fread_s(Data, m_MetaData.size, m_MetaData.size, 1, file);
		for (int i = 0; i < m_MetaData.numOfChildren; i++)
		{
			m_children.push_back(new PackElement(file));
		}
    }
	PackElement(PackElement const& arg) :m_MetaData(arg.m_MetaData)
	{
		Copy(Data, arg);
	}
	PackElement(ID_Type _ID, size_t _size, void* pData)
	{
		Copy(Data, _size, pData);
		m_MetaData.ID = _ID;
		m_MetaData.size = _size;
	}
	template <typename T>
	PackElement(ID_Type _ID, T obj)
	{
		m_MetaData.ID = _ID;
		m_MetaData.size = sizeof(T);
		Copy(Data, obj);
	}
	template<typename T>
	PackElement(ID_Type _ID, T* c_str)
	{
		m_MetaData.ID = _ID;
		m_MetaData.size = strlen(c_str) + 1;
		Copy(Data,m_MetaData.size, c_str);
	}
	PackElement(ID_Type _ID, std::string& str)
	{
		m_MetaData.ID = _ID;
		m_MetaData.size = str.size() + 1;
		Copy(Data, m_MetaData.size, str.c_str());
	}

	void operator=(PackElement const& arg)
	{
		Copy(m_MetaData, arg.m_MetaData);
		Copy(Data, arg.m_MetaData.size, arg.Data);
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
	void Get(T& obj)
	{
		if (m_MetaData.size != sizeof(T))
			throw std::exception("Sizes mismatch!");

		memcpy(&obj, Data, m_MetaData.size);
		return;
	}
	template <>
	void Get <std::string> (std::string& obj)
	{
		obj = static_cast<char*>(Data);
		return;
	}
	template <>
	void Get <std::wstring> (std::wstring& obj)
	{
		obj = static_cast<wchar_t*>(Data);
		return;
	}
	template <class T>
	void Get(T*& ptr)
	{
		ptr = static_cast<T*>(malloc(m_MetaData.size));
		memcpy(ptr, Data, m_MetaData.size);
		return;
	}
	PackElement* Find(ID_Type ID)
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
		for (auto child : m_children)
		{
			if (child->m_MetaData.ID == _child->m_MetaData.ID)
			{
				delete child;
				child = _child;
				return;
			}
		}
		m_children.push_back(_child);
		m_MetaData.numOfChildren++;
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
};

template<class DataOwnershipPolicy, class ID_Type>
class PackFile
{
public:
	typedef PackElement<DataOwnershipPolicy, ID_Type> LocalElement;
protected:
	const char * m_fileExtention;
	FILE* m_file;
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
	void operator<<(LocalElement *elem)
	{
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
		m_Elements.push_back(elem);
	}
	bool operator>>(LocalElement*& OutElem)
	{
		static int counter(0);
		if (counter < m_Elements.size())
		{
			OutElem = m_Elements[counter++];
			return true;
		}
		OutElem = nullptr;
		counter = 0;
		return false;

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
	void ToDisk()override final
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
    }
};

template<class DataOwnershipPolicy>
class ProjectPackFile : public PackFile < DataOwnershipPolicy, ProjectPackElementID >
{
public:
	ProjectPackFile(){ m_fileExtention = ".asp"; }
};