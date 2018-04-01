#include "Session.h"
#include "serialization/Utils.h"

using std::experimental::string_view;
using namespace std;

namespace ASL
{
	Session::Session() : processor()
	{
	}

	void Session::set_shader_processor(const IShaderCodeProcessorRef & processor) {
        if (auto processor_ptr = processor.lock()) {
            pipeline_project.subsystem = processor_ptr->name();
            this->processor = processor;
        }
	}

	void Session::set_pipeline_name(const std::string & name) {
		pipeline_project.name = name;
	}



	void Session::SaveShaderFile(const char* filename)
	{
        to_file_msg_pack(filename, compiled_pipeline);
	}

	void Session::SaveProjectFile(const string & file_name)
	{

	}

	const std::vector<ShaderSource> & Session::get_shaders() const
	{
		return pipeline_project.source.shaders;
	}

	bool Session::CheckFileExists(const string & file_name)
	{
		auto file = OS::Files::open_file(file_name, "rb");
		if (file)
		{
			fclose(file);
			return true;
		}
		return false;
	}

	void Session::OpenProjectFile(const string & file_name)
	{
		from_file_msg_pack(file_name, pipeline_project);
	}

	void Session::write_element(ShaderCompiled && shader)
	{
		compiled_pipeline.shaders.push_back(move(shader));
	}

	void Session::clear_compiled_elements()
	{
		compiled_pipeline.shaders.clear();
	}

	Session::~Session()
	{
	}

	const string & Session::get_pipeline_language() const {
		return pipeline_project.language;
	}

    const string & Session::get_pipeline_subsystem() const {
		return pipeline_project.subsystem;
	}

	const std::string & Session::get_pipeline_name() const {
		return pipeline_project.name;
	}

	void Session::compile() {
        auto processor = this->processor.lock();
		if (!processor)
			throw approx_exception("Shader code processor is not present in this session.", "ASL::Session::Compile");

		processor->compile(pipeline_project, compiled_pipeline);
	}

    void Session::set_pipeline_language(const std::string & language) {
        pipeline_project.language = language;
    }
}