#include "Controller.h"
#include <RendererCommonDefinitions.h>
#include "ShaderCodeProcessing.h"
#include "Session.h"
#include "ShaderPart.h"
#include "ViewSessionInfo.h"
#include <qvector.h>
#include "ShaderParamInfo.h"
#include "TextureInfo.h"

using namespace MatInterface;

namespace ASL
{
	Session* Controller::UpdateDataAndGetSession(const ViewSessionInfo& visSession)
	{
		auto res = m_Sessions.find(visSession.m_SessionID);
		if (res == m_Sessions.end())
			throw std::runtime_error("Wrong Session ID (class Controller in ApproxShaderLab)");

		Session* session = res->second;
		session->setShaderModel(visSession.m_SM);
		session->setShaderName(visSession.m_shaderName.toStdString());
		session->setShaderParams(visSession.m_Params.toStdVector());
		session->setShaderTextures(visSession.m_Textures.toStdVector());

		for (auto part : visSession.m_ShaderParts)
		{
			ShaderPart& lpart = session->partByType(part.Shader_Type);
			lpart.EntryPoint = part.entryPoint.toStdString();
			lpart.Str_code = part.qStr_code.toStdString();
			lpart.BuffersInfo = part.buffersInfo.toStdVector();
			lpart.ParamsIDs = part.paramIDs.toStdVector();
			lpart.TextureSlots = part.textureSlots.toStdVector();
		}
		return session;
	}

	template<class T>
	inline QVector<T> fromStdVector(const vector<T>& In)
	{
		return QVector<T>::fromStdVector(In);
	}

	inline void ReadSession(const Session& In, ViewSessionInfo& Out)
	{
		Out.m_shaderName = QString(In.ShaderName());
		Out.m_SM = In.ShaderModel();
		Out.m_ShaderParts.clear();
		Out.m_Params = fromStdVector(In.ShaderParameters());
		Out.m_Textures = fromStdVector(In.ShaderTextures());

		for (auto part : In.ShaderParts())
		{
			ViewShaderPartInfo partInfo;
			partInfo.entryPoint = QString(part.EntryPoint.c_str());
			partInfo.qStr_code = QString(part.Str_code.c_str());
			partInfo.Shader_Type = part.shader_type;
			partInfo.buffersInfo = fromStdVector(part.BuffersInfo);	
			partInfo.paramIDs = fromStdVector(part.ParamsIDs);
			partInfo.textureSlots = fromStdVector(part.TextureSlots);
			Out.m_ShaderParts.push_back(partInfo);
		}
	}

	int Controller::OpenSession()
	{
		static int number(1);
		auto res = m_Sessions.find(number);
		while (res != m_Sessions.end())
		{
			res = m_Sessions.find(number++);
		}
		Session *newSession = new Session;
		m_Sessions.insert({number, newSession});
		return number;
	}

	Errors Controller::Compile(const ViewSessionInfo& info)
	{
		return ShaderCodeProcessing::Compile(UpdateDataAndGetSession(info));
	}

	void Controller::SetParentWidget(QWidget* parent)
	{
		m_view->setParent(parent);
	}

	void Controller::SaveShader(const ViewSessionInfo& info)
	{
		UpdateDataAndGetSession(info)->SaveShaderFile(info.m_shaderFilename.toStdString().c_str());
	}

	void Controller::LoadProject(ViewSessionInfo& info)
	{
		Session* session = UpdateDataAndGetSession(info);
		session->OpenProjectFile(info.m_projectFilename.toStdString().c_str());
		ReadSession(*session, info);
	}

	void Controller::SaveProject(const ViewSessionInfo& info)
	{
		UpdateDataAndGetSession(info)->SaveProjectFile(info.m_projectFilename.toStdString().c_str());
	}

	void Controller::CloseSession(const ViewSessionInfo& info)
	{
		CloseSession(info.m_SessionID);
	}

	void Controller::CloseSession(const int ID)
	{
		auto res = m_Sessions.find(ID);
		if (res == m_Sessions.end())
			throw std::runtime_error("Wrong Session ID passed during CloseSession (class Controller in ApproxShaderLab)");
		delete res->second;
		m_Sessions.erase(res);
	}

	bool Controller::CheckFileExists(QString& fileName)
	{
		auto tmp = fileName.toStdString();
		return Session::CheckFileExists(tmp.c_str());
	}

	void Controller::Run()
	{

		m_view->show();

	}

	void Controller::Shutdown()
	{
		if (m_view)
		{
			m_view->close();
			delete m_view;
			m_view = nullptr;
		}
		for (auto session : m_Sessions)
		{
			delete session.second;
			session.second = nullptr;
		}
		m_Sessions.clear();
	}

	void Controller::SetMaterialInterfaceInfo(MatInterface::MaterialInterfaceInfo const& MI_Info)
	{
		std::vector < MaterialVarInfo > propertyInfo;
		propertyInfo.reserve(MI_Info.Vars.size());
		for (auto property : MI_Info.Vars)
		{
			MaterialVarInfo info;
			info.type = QString::fromStdString(property->type());
			info.name = QString::fromStdString(property->Name());
			info.description = QString::fromStdString(property->Description());
			info.ID = property->ID();
			info.group = static_cast<ASL::VarGroup>(property->Group());
			propertyInfo.push_back(info);
		}
		m_view->SetMaterialInterfaceInfo(propertyInfo, QString::fromStdString(MI_Info.Ver));
	}

	Controller::Controller()
	{
		m_view = new ShaderLabGUI(this);
	}


	Controller::~Controller()
	{
		Shutdown();
	}

	IApproxShaderLabControl* GetASLControl()
	{
		static Controller singleton;
		return &singleton;
	}
}