#include "SPIRVShaderCodeProcessor.h"
#include "Session.h"
#include "ShaderPart.h"

#include <Logger.h>
#include <Utility.h>
#include <SPIRV/GlslangToSpv.h>
#include <boost/locale/conversion.hpp>


#include <Bimap.h>

using namespace std;
using namespace spvtools;
using namespace glslang;


namespace ASL
{
    bimap<string, spv_target_env> available_versions {
        {"SPIR-V 1.0", SPV_ENV_UNIVERSAL_1_0},
        {"Vulkan 1.0", SPV_ENV_VULKAN_1_0},
        {"SPIR-V 1.1", SPV_ENV_UNIVERSAL_1_1},
        {"OpenCL 2.1", SPV_ENV_OPENCL_2_1},
        {"OpenCL 2.2", SPV_ENV_OPENCL_2_2},
        {"OpenGL 4.0", SPV_ENV_OPENGL_4_0},
        {"OpenGL 4.1", SPV_ENV_OPENGL_4_1},
        {"OpenGL 4.2", SPV_ENV_OPENGL_4_2},
        {"OpenGL 4.3", SPV_ENV_OPENGL_4_3},
        {"OpenGL 4.5", SPV_ENV_OPENGL_4_5},
    };

    struct Configuration {
        EShMessages messages = EShMsgDefault;
        bimap<ShaderType, EShLanguage> shader_types = {};
    };

    unordered_map<string, Configuration> available_configurations {
        {"GLSL",
            {
                (EShMessages) (EShMsgSpvRules | EShMsgVulkanRules),
                {
                    {VS, EShLangVertex},
                    {PS, EShLangFragment},
                    {GS, EShLangGeometry},
                    {CS, EShLangCompute},
                }
            }
        },
        {"HLSL",
            {
                (EShMessages) (EShMsgSpvRules | EShMsgReadHlsl),
                {
                    {VS, EShLangVertex},
                    {PS, EShLangFragment},
                    {GS, EShLangGeometry},
                    {CS, EShLangCompute},
                }
            }
        }
    };

    map<ShaderType, string> shader_types_description {
        {VS, "вершинный"},
        {PS, "пиксельный"},
        {GS, "геометрический"},
        {CS, "вычислительный"},
        {DS, "доменный"},
        {HS, "поверхностный"},
    };

    const TBuiltInResource DefaultTBuiltInResource = {
        .maxLights = 32,
        .maxClipPlanes = 6,
        .maxTextureUnits = 32,
        .maxTextureCoords = 32,
        .maxVertexAttribs = 64,
        .maxVertexUniformComponents = 4096,
        .maxVaryingFloats = 64,
        .maxVertexTextureImageUnits = 32,
        .maxCombinedTextureImageUnits = 80,
        .maxTextureImageUnits = 32,
        .maxFragmentUniformComponents = 4096,
        .maxDrawBuffers = 32,
        .maxVertexUniformVectors = 128,
        .maxVaryingVectors = 8,
        .maxFragmentUniformVectors = 16,
        .maxVertexOutputVectors = 16,
        .maxFragmentInputVectors = 15,
        .minProgramTexelOffset = -8,
        .maxProgramTexelOffset = 7,
        .maxClipDistances = 8,
        .maxComputeWorkGroupCountX = 65535,
        .maxComputeWorkGroupCountY = 65535,
        .maxComputeWorkGroupCountZ = 65535,
        .maxComputeWorkGroupSizeX = 1024,
        .maxComputeWorkGroupSizeY = 1024,
        .maxComputeWorkGroupSizeZ = 64,
        .maxComputeUniformComponents = 1024,
        .maxComputeTextureImageUnits = 16,
        .maxComputeImageUniforms = 8,
        .maxComputeAtomicCounters = 8,
        .maxComputeAtomicCounterBuffers = 1,
        .maxVaryingComponents = 60,
        .maxVertexOutputComponents = 64,
        .maxGeometryInputComponents = 64,
        .maxGeometryOutputComponents = 128,
        .maxFragmentInputComponents = 128,
        .maxImageUnits = 8,
        .maxCombinedImageUnitsAndFragmentOutputs = 8,
        .maxCombinedShaderOutputResources = 8,
        .maxImageSamples = 0,
        .maxVertexImageUniforms = 0,
        .maxTessControlImageUniforms = 0,
        .maxTessEvaluationImageUniforms = 0,
        .maxGeometryImageUniforms = 0,
        .maxFragmentImageUniforms = 8,
        .maxCombinedImageUniforms = 8,
        .maxGeometryTextureImageUnits = 16,
        .maxGeometryOutputVertices = 256,
        .maxGeometryTotalOutputComponents = 1024,
        .maxGeometryUniformComponents = 1024,
        .maxGeometryVaryingComponents = 64,
        .maxTessControlInputComponents = 128,
        .maxTessControlOutputComponents = 128,
        .maxTessControlTextureImageUnits = 16,
        .maxTessControlUniformComponents = 1024,
        .maxTessControlTotalOutputComponents = 4096,
        .maxTessEvaluationInputComponents = 128,
        .maxTessEvaluationOutputComponents = 128,
        .maxTessEvaluationTextureImageUnits = 16,
        .maxTessEvaluationUniformComponents = 1024,
        .maxTessPatchComponents = 120,
        .maxPatchVertices = 32,
        .maxTessGenLevel = 64,
        .maxViewports = 16,
        .maxVertexAtomicCounters = 0,
        .maxTessControlAtomicCounters = 0,
        .maxTessEvaluationAtomicCounters = 0,
        .maxGeometryAtomicCounters = 0,
        .maxFragmentAtomicCounters = 8,
        .maxCombinedAtomicCounters = 8,
        .maxAtomicCounterBindings = 1,
        .maxVertexAtomicCounterBuffers = 0,
        .maxTessControlAtomicCounterBuffers = 0,
        .maxTessEvaluationAtomicCounterBuffers = 0,
        .maxGeometryAtomicCounterBuffers = 0,
        .maxFragmentAtomicCounterBuffers = 1,
        .maxCombinedAtomicCounterBuffers = 1,
        .maxAtomicCounterBufferSize = 16384,
        .maxTransformFeedbackBuffers = 4,
        .maxTransformFeedbackInterleavedComponents = 64,
        .maxCullDistances = 8,
        .maxCombinedClipAndCullDistances = 8,
        .maxSamples = 4,
        .limits = {
           .nonInductiveForLoops = 1,
           .whileLoops = 1,
           .doWhileLoops = 1,
           .generalUniformIndexing = 1,
           .generalAttributeMatrixVectorIndexing = 1,
           .generalVaryingIndexing = 1,
           .generalSamplerIndexing = 1,
           .generalVariableIndexing = 1,
           .generalConstantMatrixVectorIndexing = 1,
        }
    };

    void ASL::SPIRVShaderCodeProcessor::compile(const PipelineProject & project, PipelineCompiled & compiled) const {

        const auto & config = available_configurations[project.language];
        const auto & source = project.source;

        glslang::InitializeProcess();
        scope_exit( glslang::FinalizeProcess(); )

        TProgram program;
        auto resources = DefaultTBuiltInResource;
        EShMessages messages = config.messages;
        vector<shared_ptr<TShader>> shaders;

        for (auto & shader: source.shaders) {
            assert(shader.type != NONE);
            auto stage = config.shader_types.at(shader.type);

            if (stage == EShLangCount) {
                auto shader_type_name = shader_types_description[shader.type];
                throw approx_exception(
                    format("%s тип шейдера не поддерживается", boost::locale::to_title(shader_type_name).c_str()));
            }

            auto shader_program = make_shared<TShader>(stage);
            shaders.push_back(shader_program);

            auto code = shader.code.c_str();
            shader_program->setStrings(&code, 1);
            if (!shader.entry_point.empty())
                shader_program->setEntryPoint(shader.entry_point.c_str());

            if (!shader_program->parse(&resources, 110, false, messages)) {
                throw approx_exception("Ошибка при парсинге");
            }

            program.addShader(shader_program.get());
        }

        program.link(messages);

        msgpack::sbuffer bytes;
        for (int stage = 0; stage < EShLangCount; ++stage) {
            auto e_stage = static_cast<EShLanguage>(stage);
            auto ir = program.getIntermediate(e_stage);
            if (ir) {
                spv::SpvBuildLogger logger;
                vector<uint32_t> binary;

                GlslangToSpv(*ir, binary, &logger);
                Logger::instance().log(logger.getAllMessages());
                auto & shader = source.get_shader_by_type(config.shader_types.at(e_stage));

                compiled.shaders.emplace_back();
                auto compiled_shader = compiled.shaders.back();
                *static_cast<ShaderCommon*>(&compiled_shader) = *static_cast<const ShaderCommon*>(&shader);
                compiled_shader.bytes = move(binary);

                compiled_shader.parameters_ids.reserve(shader.parameters.size());
                for (const auto & parameter: shader.parameters) {
                    compiled_shader.parameters_ids.push_back(parameter.id);
                }

                compiled_shader.texture_slots.reserve(shader.textures.size());
                for (const auto & texture: shader.textures) {
                    compiled_shader.texture_slots.push_back(texture.slot);
                }
            }
        }
    }

    ShaderProcessorDescription SPIRVShaderCodeProcessor::describe() const {
        ShaderProcessorDescription description {
            .subsystem_name = name()
        };
        for (auto config : available_configurations) {
            description.available_languages.push_back(config.first);
        }

        return description;
    }

    bool SPIRVShaderCodeProcessor::has_version(const string &version) const {
        return available_versions.has(version);
    }

    string SPIRVShaderCodeProcessor::name() const {
        return "SPIR-V";
    }
}