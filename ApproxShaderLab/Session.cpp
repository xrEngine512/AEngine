#include "Session.h"
#include "ShaderPart.h"
#include "ShaderParamInfo.h"
#include "TextureInfo.h"

#define IDENTIFY(Type) case ProjectPackElementID::Type: return Type

namespace ASL
{
	Session::Session() :m_SM(SM_NONE)
	{
		m_file.AllowDuplicatesFor(INPUT_LAYOUT_ELEMENT);
	}

	inline ProjectPackElementID toID(Shader_Type type)
	{
		switch (type)
		{
		case ASL::ST_NONE:	return ProjectPackElementID::NONE;
		case ASL::VS:		return ProjectPackElementID::VS;
		case ASL::PS:		return ProjectPackElementID::PS;
		case ASL::GS:		return ProjectPackElementID::GS;
		case ASL::CS:		return ProjectPackElementID::CS;
		case ASL::DS:		return ProjectPackElementID::DS;
		case ASL::HS:		return ProjectPackElementID::HS;
		}
	}

	inline Shader_Type toST(ProjectPackElementID ID)
	{
		switch (ID)
		{
			IDENTIFY(VS);
			IDENTIFY(PS);
			IDENTIFY(CS);
			IDENTIFY(HS);
			IDENTIFY(DS);
			IDENTIFY(GS);
		default: return ST_NONE;
		}
	}

	void Session::setShaderParams(vector<ShaderParamInfo>& params)
	{
		m_ShaderParams = params;
	}

	void Session::setShaderTextures(vector<TextureInfo>& textures)
	{
		m_ShaderTextures = textures;
	}

	ShaderPart& Session::partByType(Shader_Type type)
	{
		for (auto& part : m_ShaderParts)
		{
			if (part.Shader_Type == type)
			{
				return part;
			}
		}
		m_ShaderParts.push_back(ShaderPart());
		ShaderPart& part = m_ShaderParts.back();
		part.Shader_Type = type;
		return part;
	}

	ShaderPart* Session::partByIndex(int index)
	{
		if (index >= m_ShaderParts.size())
			return nullptr;
		return &m_ShaderParts[index];
	}

	void Session::SaveShaderFile(const char* filename)
	{
		m_file(filename);
		m_file.ToDisk();
		m_file.CloseFile();
	}

	void Session::SaveProjectFile(const char* filename)
	{
		m_project << new ProjectElement(ProjectPackElementID::SHADER_NAME, m_shaderName);
		m_project << new ProjectElement(ProjectPackElementID::SHADER_MODEL, m_SM);
		m_project.AllowDuplicatesFor(ProjectPackElementID::PARAMETER);
		m_project.AllowDuplicatesFor(ProjectPackElementID::TEXTURE);
		for (auto param : m_ShaderParams)
		{
			m_project << new ProjectElement(ProjectPackElementID::PARAMETER, dynamic_cast<ISaveData*>(&param));
		}
		for (auto texture : m_ShaderTextures)
		{
			m_project << new ProjectElement(ProjectPackElementID::TEXTURE, dynamic_cast<ISaveData*>(&texture));
		}
		//Saving source code and entry points
		for (auto part : m_ShaderParts)
		{
			ProjectElement* newElem = new ProjectElement(toID(part.Shader_Type), part.Str_code);
			newElem->AllowDuplicatesFor(ProjectPackElementID::APPROX_VAR_BUFFER_INFO);
			*newElem << new ProjectElement(ProjectPackElementID::ENTRY_POINT, part.EntryPoint);
			for (auto bufferInfo : part.BuffersInfo)
			{
				*newElem << new ProjectElement(ProjectPackElementID::APPROX_VAR_BUFFER_INFO, dynamic_cast<ISaveData*>(&bufferInfo));
			}
			m_project << newElem;
		}

		m_project(filename);
		m_project.ToDisk();
		m_project.CloseFile();
	}

	const std::vector<ShaderPart>& Session::ShaderParts() const
	{
		return m_ShaderParts;
	}

	bool Session::CheckFileExists(const char* filename)
	{
		FILE* file;
		fopen_s(&file, filename, "rb");
		if (file)
		{
			fclose(file);
			return true;
		}
		return false;
	}

	bool Session::CheckFileExists(const wchar_t* filename)
	{
		FILE* file;
		_wfopen_s(&file, filename, L"rb");
		if (file)
		{
			fclose(file);
			return true;
		}
		return false;
	}

	void Session::OpenProjectFile(const char* filename)
	{
		m_project.ClearElements();
		m_project(filename,false);
		m_project.FromDisk();
		m_project.CloseFile();
		ProjectElement* element = m_project.Find(ProjectPackElementID::SHADER_MODEL);
		
		element->Get(m_SM);
		
		element = m_project.Find(ProjectPackElementID::SHADER_NAME);
		
		element->Get(m_shaderName);

		m_ShaderParts.clear();

		while (m_project >> element)
		{
			try
			{
				Shader_Type ST = toST(element->m_MetaData.ID);
				if (ST != ST_NONE)
				{
					ShaderPart newPart;

					element->Find(ProjectPackElementID::ENTRY_POINT)->Get(newPart.EntryPoint);
					for (auto subElement : element->FindMany(ProjectPackElementID::APPROX_VAR_BUFFER_INFO))
					{
						RuntimeBufferInfo bufInfo;
						subElement->Get(dynamic_cast<ISaveData*>(&bufInfo));
						newPart.BuffersInfo.push_back(bufInfo);
					}
					element->Get(newPart.Str_code);
					newPart.Shader_Type = ST;
					m_ShaderParts.push_back(newPart);
				}
			}
			catch (ApproxException&)
			{
				m_project.Reset();
				throw;
			}
		}
	}

	void Session::writeElement(ShaderElement* element)
	{
		m_file << element;
	}

	Session::~Session()
	{
	}
}