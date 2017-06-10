#pragma once
#include "Enums.h"
#include "IShaderCodeProcessor.h"

class QString;

namespace ASL
{
	struct ViewSessionInfo;
	interface IControllerSession
	{
		virtual int OpenSession() = 0;
		virtual void compile(const ViewSessionInfo &) = 0;
		virtual void SaveShader(const ViewSessionInfo&) = 0;
		virtual void SaveProject(const ViewSessionInfo&) = 0;
		virtual void LoadProject(ViewSessionInfo&) = 0;
		virtual bool CheckFileExists(QString& fileName) = 0;
		virtual void CloseSession(const ViewSessionInfo&) = 0;
		virtual void CloseSession(const int ID) = 0;
        virtual std::vector<IShaderCodeProcessorPtr> get_available_shader_processors() const = 0;
		virtual ~IControllerSession(){};
	};
}