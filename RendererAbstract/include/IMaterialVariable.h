#pragma once

namespace MatInterface
{
	enum VarGroup : char { SCENE_CONSTANT, SHADER_PARAMS, PER_FRAME, PER_OBJECT };

	interface IMaterialVariable
	{
		virtual void* Ptr() = 0;
		virtual int Size() const = 0;
		virtual int ID() const = 0;
		virtual const std::string& Description() const = 0;
		virtual const std::string& Name() const = 0;
		virtual VarGroup Group() const = 0;
		virtual std::string type() const noexcept = 0;
		virtual ~IMaterialVariable(){};
	};
}