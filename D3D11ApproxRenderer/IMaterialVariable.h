#pragma once

namespace MatInterface
{
	enum VarGroup : __int8 { SCENE_CONSTANT, SHADER_PARAMS, PER_FRAME, PER_OBJECT };

	__interface IMaterialVariable
	{
		void* Ptr();
		int Size()const;
		int ID()const;
		const char* Description()const;
		const char* Name()const;
		VarGroup Group()const;
		const char* Type()const;
	};
}