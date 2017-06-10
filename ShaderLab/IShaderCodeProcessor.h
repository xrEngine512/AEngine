#ifndef APPROXSHADERLAB_H
#define APPROXSHADERLAB_H
#include "Enums.h"

#include <vector>
#include <string>
#include <memory>

namespace ASL
{
	struct ShaderPart;
	class Session;

    struct ShaderProcessorDescription {
        std::string subsystem_name;
        std::vector<std::string> available_languages;
    };

	interface IShaderCodeProcessor
    {
        virtual void compile(Session *) const = 0;
        virtual ShaderProcessorDescription describe() const = 0;
        virtual bool has_version(const std::string &version) const = 0;
        virtual std::string name() const = 0;
    };

    typedef std::shared_ptr<IShaderCodeProcessor> IShaderCodeProcessorPtr;
    typedef std::weak_ptr<IShaderCodeProcessor> IShaderCodeProcessorRef;
}
#endif // APPROXSHADERLAB_H
