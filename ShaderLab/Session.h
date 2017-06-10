#pragma once
#include "Enums.h"
#include "ShaderPackFile.h"
#include "IShaderCodeProcessor.h"
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

	class Session
	{
	public:
		Session();

		void setShaderProcessor(const IShaderCodeProcessorRef& processor);
        void setShaderVersion(const std::string& version);
		void setShaderName(const std::string& name);
		void setShaderParams(const vector<ShaderParamInfo>& params);
		void setShaderTextures(const vector<TextureInfo>& textures);

		void compile();

		std::string get_shader_language() const;
		std::string get_shader_subsystem() const;
		const std::string& ShaderName() const;
		const vector<ShaderPart>& ShaderParts() const;
		ShaderPart& partByType(Shader_Type);
		ShaderPart* partByIndex(int);
		const vector<ShaderParamInfo>& ShaderParameters() const{ return m_ShaderParams; }
		const vector<TextureInfo>& ShaderTextures() const{ return m_ShaderTextures; }
		static bool CheckFileExists(const std::experimental::string_view& filename);
		void SaveShaderFile(const char* filename);
		void SaveProjectFile(const char* filename);
		void OpenProjectFile(const char* filename);
		void write_element(ShaderElement *);
		void clearCompiledElements();
		~Session();	

	private:
        IShaderCodeProcessorRef processor;
        std::string shader_subsystem;
        std::string shader_language;
		std::string m_shaderName;
		ProjectFile m_project;
		ShaderFile m_file;
		vector<ShaderPart> m_ShaderParts;
		vector<ShaderParamInfo> m_ShaderParams;
		vector<TextureInfo> m_ShaderTextures;
	};

}