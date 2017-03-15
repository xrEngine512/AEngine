#pragma once
#include "Enums.h"
#include "ShaderPackFile.h"
#include <vector>
#include <experimental/string_view>


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

		void setShaderModel(Shader_Model model){ m_SM = model; }
		void setShaderName(const string& name){ m_shaderName = name;}
		void setShaderParams(const vector<ShaderParamInfo>& params);
		void setShaderTextures(const vector<TextureInfo>& textures);


		Shader_Model ShaderModel() const{return m_SM;}
		const char* ShaderName() const{return m_shaderName.c_str();}
		const vector<ShaderPart>& ShaderParts() const;

		ShaderPart& partByType(Shader_Type);
		ShaderPart* partByIndex(int);
		const vector<ShaderParamInfo>& ShaderParameters() const{ return m_ShaderParams; }
		const vector<TextureInfo>& ShaderTextures() const{ return m_ShaderTextures; }
		static bool CheckFileExists(const std::experimental::string_view& filename);
		void SaveShaderFile(const char* filename);
		void SaveProjectFile(const char* filename);
		void OpenProjectFile(const char* filename);
		void writeElement(ShaderElement*);
		void clearCompiledElements();
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