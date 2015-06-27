#ifndef APPROXSHADERLAB_H
#define APPROXSHADERLAB_H
#include "Enums.h"

namespace ASL
{
	struct ShaderPart;
	class Session;    
   
    class HLSLCodeProcessing
    {
    public:
        static Errors Compile(Session*);
	private:
		static const char* getTarget(Shader_Type, Shader_Model);
		static Errors GenerateInputLayout(Session* session, ShaderPart& part);
    };
}
#endif // APPROXSHADERLAB_H
