#pragma once
#include "IApproxShaderLabControl.h"
#include "IApproxShaderLabExternalControl.h"
#include "IControllerSession.h"
//#include "approxshaderlab.h"
#include "shaderlabgui.h"

namespace ASL
{
	class Session;
}

class Controller : public IApproxShaderLabControl, public IApproxShaderLabExternalControl, public IControllerSession
{
   // ApproxShaderLab m_model;
    ShaderLabGUI *m_view;
	std::map<int, ASL::Session*> m_Sessions;
	ASL::Session* UpdateDataAndGetSession(const ApproxViewSessionInfo&);
public:
	int OpenSession()override final;
	ASL::Errors Compile(const ApproxViewSessionInfo&) override final;
    void SetParentWidget(QWidget*)override final;
	void SaveShader(const ApproxViewSessionInfo&)override final;
	void LoadProject(ApproxViewSessionInfo&)override final;
	void SaveProject(const ApproxViewSessionInfo&)override final;
	bool CheckFileExists(QString& fileName)override final;
	void CloseSession(const ApproxViewSessionInfo&)override final;
	void CloseSession(const int ID)override final;
    void Run()override final;
    void Shutdown()override final;
	void SetSceneProperties(std::vector <IMaterialVariable*>const&)override final;
    Controller();
    virtual ~Controller();
};

IApproxShaderLabControl* GetASLControl();
