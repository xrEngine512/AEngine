#pragma once
#include "Enums.h"

class QString;

namespace ASL
{
	struct ViewSessionInfo;
	__interface IControllerSession
	{
		int OpenSession();
		ASL::Errors Compile(const ViewSessionInfo&);
		void SaveShader(const ViewSessionInfo&);
		void SaveProject(const ViewSessionInfo&);
		void LoadProject(ViewSessionInfo&);
		bool CheckFileExists(QString& fileName);
		void CloseSession(const ViewSessionInfo&);
		void CloseSession(const int ID);
	};
}