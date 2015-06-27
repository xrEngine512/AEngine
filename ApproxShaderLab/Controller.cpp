#include "Controller.h"
#include <IMaterialVariable.h>
#include "HLSLCodeProcessing.h"
#include "Session.h"
#include "ShaderPart.h"
#include "ApproxViewSessionInfo.h"

//inline char* QStringToChar(const QString & qstr);
using namespace ASL;

Session* Controller::UpdateDataAndGetSession(const ApproxViewSessionInfo& visSession)
{
	auto res = m_Sessions.find(visSession.m_SessionID);
	if (res == m_Sessions.end())
		throw std::exception("Wrong Session ID (class Controller in ApproxShaderLab)");

	Session* session = res->second;
	session->setShaderModel(visSession.m_SM);
	session->setShaderName(visSession.m_shaderName.toStdString());
	for (auto part : visSession.m_ShaderParts)
	{
		ShaderPart& lpart = session->partByType(part.Shader_Type);
		lpart.EntryPoint = part.entryPoint.toStdString();
		lpart.Str_code = part.qStr_code.toStdString();
	}
	return session;
}

inline void ReadSession(const Session& In, ApproxViewSessionInfo& Out)
{
	Out.m_shaderName = QString(In.ShaderName());
	Out.m_SM = In.ShaderModel();
	Out.m_ShaderParts.clear();
	for (auto part : In.ShaderParts())
	{
		ApproxViewShaderPartInfo partInfo;
		partInfo.entryPoint = QString(part.EntryPoint.c_str());
		partInfo.qStr_code = QString(part.Str_code.c_str());
		partInfo.Shader_Type = part.Shader_Type;
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
	m_Sessions.insert(std::pair<int, Session*>(number,newSession));
	return number;
}

Errors Controller::Compile(const ApproxViewSessionInfo& info)
{
	return HLSLCodeProcessing::Compile(UpdateDataAndGetSession(info));
}

void Controller::SetParentWidget(QWidget* parent)
{
    m_view->setParent(parent);
}

void Controller::SaveShader(const ApproxViewSessionInfo& info)
{
	UpdateDataAndGetSession(info)->SaveShaderFile(info.m_shaderFilename.toStdString().c_str());
}

void Controller::LoadProject(ApproxViewSessionInfo& info)
{
	Session* session = UpdateDataAndGetSession(info);
	session->OpenProjectFile(info.m_projectFilename.toStdString().c_str());
	ReadSession(*session, info);
}

void Controller::SaveProject(const ApproxViewSessionInfo& info)
{
	UpdateDataAndGetSession(info)->SaveProjectFile(info.m_projectFilename.toStdString().c_str());
}

void Controller::CloseSession(const ApproxViewSessionInfo& info)
{
	CloseSession(info.m_SessionID);
}

void Controller::CloseSession(const int ID)
{
	auto res = m_Sessions.find(ID);
	if (res == m_Sessions.end())
		throw std::exception("Wrong Session ID passed during CloseSession (class Controller in ApproxShaderLab)");
	delete res->second;
	m_Sessions.erase(res);
}

bool Controller::CheckFileExists(QString& fileName)
{
	auto tmp = fileName.toStdWString();
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

void Controller::SetSceneProperties(std::vector<IMaterialVariable*>const& properties)
{
    auto propertyInfo = new std::vector<ScenePropertyVisualInfo>;
    propertyInfo->reserve(properties.size());
    for (auto property : properties)
    {
        ScenePropertyVisualInfo info;
        info.type = QString(property->Type());
        info.name = QString(property->Name());
        info.description = QString(property->Description());
        propertyInfo->push_back(info);
    }
    m_view->SetSceneProperties(*propertyInfo);
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

/*inline char* QStringToChar(const QString & qstr)
{
	auto tmpStr = qstr.toStdString();
	int size = tmpStr.size();
	char* c_buf = new char[size + 1];
	strcpy_s(c_buf, size + 1, tmpStr.c_str);
	return c_buf;
}*/