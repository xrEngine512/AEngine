//
// Created by islam on 18.03.17.
//

#pragma once

#include "IShaderCodeProcessor.h"

#include <spirv-tools/libspirv.hpp>
#include <serialization/PipelineProject.h>


namespace ASL {
    class SPIRVShaderCodeProcessor : public IShaderCodeProcessor {
    public:
        void compile(const ASL::PipelineProject & project, PipelineCompiled & compiled) const override;

        ShaderProcessorDescription describe() const override;

        bool has_version(const std::string &version) const override;
        std::string name() const override;
    };

}
