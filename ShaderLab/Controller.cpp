#include "Controller.h"
#include <RendererCommonDefinitions.h>
#include "IShaderCodeProcessor.h"
#include "Session.h"
#include "ShaderPart.h"
#include "ViewSessionInfo.h"
#include "ShaderCodeProcessorEnumerator.h"

using namespace MatInterface;

namespace ASL
{
	Session* Controller::UpdateDataAndGetSession(const ViewSessionInfo& visSession)
	{
		auto res = m_Sessions.find(visSession.m_SessionID);
		if (res == m_Sessions.end())
			throw std::runtime_error("Wrong Session ID (class Controller in ApproxShaderLab)");

		Session* session = res->second;
        auto shader_language = visSession.shader_language.toStdString();
        auto shader_subsystem = visSession.shader_subsystem.toStdString();

        session->set_shader_processor(get_processor(shader_subsystem));
		session->set_pipeline_language(shader_language);
        session->set_pipeline_name(visSession.m_shaderName.toStdString());
        session->set_shader_params(visSession.m_Params.toStdVector());
		session->setShaderTextures(visSession.m_Textures.toStdVector());

		for (auto part : visSession.m_ShaderParts)
		{
			ShaderPart& lpart = session->get_shader_by_type(part.Shader_Type);
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
		Out.m_shaderName = QString::fromStdString(In.get_pipeline_name());
        Out.shader_language = QString::fromStdString(In.get_pipeline_language());
        Out.shader_subsystem = QString::fromStdString(In.get_pipeline_subsystem());
		Out.m_ShaderParts.clear();
		//Out.m_Params = fromStdVector(In.ShaderParameters());
		//Out.m_Textures = fromStdVector(In.ShaderTextures());

		for (auto shader : In.get_shaders())
		{
			ViewShaderPartInfo partInfo;
			partInfo.entryPoint = QString(shader.entry_point.c_str());
			partInfo.qStr_code = QString(shader.code.c_str());
			partInfo.Shader_Type = shader.type;
			partInfo.buffersInfo = fromStdVector(shader.buffers_info);

            partInfo.paramIDs.reserve(shader.parameters.size());
            for (const auto & parameter: shader.parameters) {
                partInfo.paramIDs.push_back(parameter.id);
            }

            partInfo.textureSlots.reserve(shader.textures.size());
            for (const auto & texture: shader.textures) {
                partInfo.textureSlots.push_back(texture.slot);
            }

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

	void Controller::compile(const ViewSessionInfo &info)
	{
        auto session = UpdateDataAndGetSession(info);

		session->compile();
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
        available_processors = enumerate_available_processors();
	}


	Controller::~Controller()
	{
		Shutdown();
	}

    vector<IShaderCodeProcessorPtr> Controller::get_available_shader_processors() const {
        return available_processors;
    }

    IShaderCodeProcessorPtr Controller::get_processor(const std::string &name) {
        for (auto processor : available_processors) {
            if (processor->name() == name) {
                return processor;
            }
        }
        return nullptr;
    }

    IApproxShaderLabControl* GetASLControl()
	{
		static Controller singleton;
		return &singleton;
	}
}