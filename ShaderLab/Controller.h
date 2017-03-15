#pragma once
#include "IApproxShaderLabControl.h"
#include "IApproxShaderLabExternalControl.h"
#include "IControllerSession.h"
#include "shaderlabgui.h"

namespace ASL
{
	class Session;

	class Controller : public IApproxShaderLabControl, public IApproxShaderLabExternalControl, public IControllerSession
	{
		ShaderLabGUI *m_view;
		std::map<int, ASL::Session*> m_Sessions;
		Session* UpdateDataAndGetSession(const ViewSessionInfo&);
	public:
		int OpenSession()override final;
		Errors Compile(const ViewSessionInfo&) override final;
		void SetParentWidget(QWidget*)override final;
		void SaveShader(const ViewSessionInfo&)override final;
		void LoadProject(ViewSessionInfo&)override final;
		void SaveProject(const ViewSessionInfo&)override final;
		bool CheckFileExists(QString& fileName)override final;
		void CloseSession(const ViewSessionInfo&)override final;
		void CloseSession(const int ID)override final;
		void Run()override final;
		void Shutdown()override final;
		void SetMaterialInterfaceInfo(MatInterface::MaterialInterfaceInfo const& MI_Info)override final;
		Controller();
		virtual ~Controller();
	};

	IApproxShaderLabControl* GetASLControl();
}