#include "SPIRVShaderCodeProcessor.h"
#include "Session.h"
#include "ShaderPart.h"

#include <Logger.h>
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
        bimap<Shader_Type, EShLanguage> shader_types;
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

    bimap<Shader_Type, EShLanguage> shader_types {

    };

    map<Shader_Type, string> shader_types_description {
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


    inline ShaderPackElementID toID(Shader_Type type)
    {
        switch (type)
        {
            case VS: return COMPILED_VS;
            case PS: return COMPILED_PS;
            case GS: return COMPILED_GS;
            case CS: return COMPILED_CS;
            case DS: return COMPILED_DS;
            case HS: return COMPILED_HS;
            default: return ID_NONE;
        }
    }

    void ASL::SPIRVShaderCodeProcessor::compile(Session *session) const {

        auto config = available_configurations[session->get_shader_language()];

        glslang::InitializeProcess();

        TProgram program;
        auto resources = DefaultTBuiltInResource;
        EShMessages messages = config.messages;
        vector<shared_ptr<TShader>> shaders;

        try {
            int partIndex = 0;
            ShaderPart *part = session->partByIndex(partIndex);

            while (part) {
                assert(part->shader_type != ST_NONE);
                auto stage = shader_types[part->shader_type];

                if (stage == EShLangCount) {
                    auto shader_type_name = shader_types_description[part->shader_type];
                    throw approx_exception(
                        format("%s тип шейдера не поддерживается", boost::locale::to_title(shader_type_name).c_str()));
                }

                auto shader = make_shared<TShader>(stage);
                shaders.push_back(shader);

                auto code = part->Str_code.c_str();
                shader->setStrings(&code, 1);
                if (!part->EntryPoint.empty())
                    shader->setEntryPoint(part->EntryPoint.c_str());

                if (!shader->parse(&resources, 110, false, messages)) {
                    throw approx_exception("Ошибка при парсинге");
                }

                program.addShader(shader.get());

                part = session->partByIndex(++partIndex);
            }

//            if (!program.link(messages)) {
//                throw approx_exception("Ошибка при линковке");
//            }

            for (int stage = 0; stage < EShLangCount; ++stage) {
                auto e_stage = static_cast<EShLanguage>(stage);
                auto ir = program.getIntermediate(e_stage);
                if (ir) {
                    spv::SpvBuildLogger logger;
                    vector<uint32_t> binary;

                    GlslangToSpv(*ir, binary, &logger);
                    Logger::instance().log(logger.getAllMessages());
                    part = &session->partByType(shader_types[e_stage]);

                    auto elem = ShaderElement::fromMemory(toID(part->shader_type), binary.size(), binary.data());
                    elem->AllowDuplicatesFor(RUNTIME_BUFFER_INFO);
                    for (auto buffer : part->BuffersInfo) {
                        *elem << ShaderElement::fromSaveData(RUNTIME_BUFFER_INFO, buffer);
                    }

                    size_t size;
                    auto ptr = AbstractSaveData::Serialize(size, part->ParamsIDs, part->TextureSlots);
                    *elem << ShaderElement::fromMemory(SHADER_SETS, size, ptr);

                    session->write_element(elem);
                }
            }
        }
        catch (approx_exception& ex) {
            glslang::FinalizeProcess();
            throw;
        }
        glslang::FinalizeProcess();
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