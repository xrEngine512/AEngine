#pragma once
#include "Enums.h"
struct ApproxViewSessionInfo;
class QString;
__interface IControllerSession
{
	int OpenSession();
	ASL::Errors Compile(const ApproxViewSessionInfo&);
	void SaveShader(const ApproxViewSessionInfo&);
	void SaveProject(const ApproxViewSessionInfo&);
	void LoadProject(ApproxViewSessionInfo&);
	bool CheckFileExists(QString& fileName);
	void CloseSession(const ApproxViewSessionInfo&);
	void CloseSession(const int ID);
};