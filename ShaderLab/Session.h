#pragma once
#include "Enums.h"
#include "ShaderPackFile.h"
#include "IShaderCodeProcessor.h"
#include <vector>
#include <experimental/string_view>
#include <serialization/PipelineCompiled.h>
#include <serialization/PipelineSource.h>
#include <serialization/PipelineProject.h>


namespace ASL
{
	struct ShaderPart;
	struct ShaderParameterInfo;
	struct TextureInfo;
	typedef ShaderPackFile<ApproxCommon::Memory::CopyDataOwnershipPolicy> ShaderFile;
	typedef ShaderFile::LocalElement ShaderElement;

	typedef ProjectPackFile<ApproxCommon::Memory::CopyDataOwnershipPolicy> ProjectFile;
	typedef ProjectFile::LocalElement ProjectElement;

	class Session
	{
	public:
		Session();

		void set_shader_processor(const IShaderCodeProcessorRef & processor);
        void set_pipeline_language(const std::string & version);
		void set_pipeline_name(const std::string & name);

		void compile();

		const std::string & get_pipeline_language() const;
		const std::string & get_pipeline_subsystem() const;
		const std::string& get_pipeline_name() const;
        const std::vector<ShaderSource> & get_shaders() const;
		const ShaderSource & get_shader_by_type(ShaderType) const;
		ShaderSource * get_shader_by_index(int);
		static bool CheckFileExists(const std::string & file_name);
		void SaveShaderFile(const char* filename);
		void SaveProjectFile(const std::string & file_name);
		void OpenProjectFile(const std::string & file_name);
		void write_element(ShaderCompiled && shader);
		void clear_compiled_elements();
		~Session();	

	private:
        IShaderCodeProcessorRef processor;
		PipelineProject pipeline_project;
		PipelineCompiled compiled_pipeline;
	};

}