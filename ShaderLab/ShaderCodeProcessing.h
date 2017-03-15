#ifndef APPROXSHADERLAB_H
#define APPROXSHADERLAB_H
#include "Enums.h"

namespace ASL
{
	struct ShaderPart;
	class Session;

	namespace ShaderCodeProcessing
    {
        Errors Compile(Session*);
    };
}
#endif // APPROXSHADERLAB_H
