#pragma once
#include "Enums.h"
#include "ShaderPackFile.h"
#include <vector>
namespace ASL
{
	struct ShaderPart;
	struct ShaderParamInfo;
	struct TextureInfo;
	typedef ShaderPackFile<ApproxCommon::Memory::CopyDataOwnershipPolicy> ShaderFile;
	typedef ShaderFile::LocalElement ShaderElement;

	typedef ProjectPackFile<ApproxCommon::Memory::CopyDataOwnershipPolicy> ProjectFile;
	typedef ProjectFile::LocalElement ProjectElement;

	using std::vector;
	using std::string;

	class Session
	{
	public:
		Session();

		Shader_Model ShaderModel() const{return m_SM;}
		const char* ShaderName() const{return m_shaderName.c_str();}

		void setShaderModel(Shader_Model model){ m_SM = model; }
		void setShaderName(const string& name){ m_shaderName = name;}
		void setShaderParams(vector<ShaderParamInfo>& params);
		void setShaderTextures(vector<TextureInfo>& textures);

		ShaderPart& partByType(Shader_Type);
		ShaderPart* partByIndex(int);
		const vector<ShaderPart>& ShaderParts() const;
		static bool CheckFileExists(const char* filename);
		static bool CheckFileExists(const wchar_t* filename);
		void SaveShaderFile(const char* filename);
		void SaveProjectFile(const char* filename);
		void OpenProjectFile(const char* filename);
		void writeElement(ShaderElement*);
		~Session();	

	private:	
		Shader_Model m_SM;
		string m_shaderName;	
		ProjectFile m_project;
		ShaderFile m_file;
		vector<ShaderPart> m_ShaderParts;
		vector<ShaderParamInfo> m_ShaderParams;
		vector<TextureInfo> m_ShaderTextures;
	};

}