#pragma once
#include "Enums.h"
#include "ShaderPackFile.h"
#include <vector>
namespace ASL
{
	struct ShaderPart;
	
	typedef ShaderPackFile<ApproxCommon::Memory::CopyDataOwnershipPolicy> ShaderFile;
	typedef ShaderFile::LocalElement ShaderElement;

	typedef ProjectPackFile<ApproxCommon::Memory::CopyDataOwnershipPolicy> ProjectFile;
	typedef ProjectFile::LocalElement ProjectElement;

	class Session
	{
	public:
		Session();

		Shader_Model ShaderModel() const{return m_SM;}
		const char* ShaderName() const{return m_shaderName.c_str();}

		void setShaderModel(Shader_Model model){ m_SM = model; }
		void setShaderName(const std::string& name){ m_shaderName = name;}

		ShaderPart& partByType(Shader_Type);
		ShaderPart* partByIndex(int);
		const std::vector<ShaderPart>& ShaderParts() const;
		static bool CheckFileExists(const char* filename);
		static bool CheckFileExists(const wchar_t* filename);
		void SaveShaderFile(const char* filename);
		void SaveProjectFile(const char* filename);
		void OpenProjectFile(const char* filename);
		void writeElement(ShaderElement*);
		~Session();	

	private:	
		Shader_Model m_SM;
		std::string m_shaderName;	
		ProjectFile m_project;
		ShaderFile m_file;
		std::vector<ShaderPart> m_ShaderParts;
	};

}