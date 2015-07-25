#include "rendererwrapper.h"
#include <QMouseEvent>


RendererWrapper::RendererWrapper(QWidget *parent) : QWidget(parent), m_input(nullptr)
{
    ui.setupUi(this);
    sys = GetSystem();
    setAttribute(Qt::WA_NoSystemBackground, true);
}

bool RendererWrapper::Initialize()
{ 
    
    if(!sys->Initialize((void*)winId(),false,1280,720,x(),y()))
        return false;
    m_input = GetInput();
    sys->GetRendererEx()->SetPosition(x(),y());
    int w, h;
    sys->GetRendererEx()->GetWndSize(w, h);
    this->resize(w,h);
    return true;
}

void RendererWrapper::Run()
{
    return sys->Run();
}

void RendererWrapper::Shutdown()
{

    return sys->Shutdown();
}

void RendererWrapper::GetCameraPos(float &x, float &y, float &z)
{
    sys->GetRendererEx()->GetScene()->GetCameraPos(x, y, z);
}

void RendererWrapper::PosUpdate()
{
    if (sys->State().Input >= SYS_INITIALIZED)
    {
        QPoint global_center = mapToGlobal(geometry().center());
        m_input->SetWndCenter(global_center.x(), global_center.y());
    }

    if (sys->State().Renderer >= SYS_INITIALIZED)
    {
        QPoint global_pos = mapToGlobal(pos());
        sys->GetRendererEx()->SetPosition(global_pos.x(), global_pos.y());
    }
}

ISystem* RendererWrapper::NativeInterface()
{
    return sys;
}

IExternalRenderObject* RendererWrapper::LoadModel(char* objFilename, const ShaderDesc& type)
{
    return sys->GetRendererEx()->GetScene()->AddRenderObject(objFilename, type);
}

void RendererWrapper::moveEvent(QMoveEvent*)
{    
    PosUpdate();
}

bool RendererWrapper::nativeEvent(const QByteArray& event_type, void* message, long* result)
{
    Q_UNUSED(result);

    if (event_type == "windows_generic_MSG")
    {
        MSG *msg = static_cast<MSG*>(message);
        if (m_input)
        {
            return !m_input->MessageHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        }

        return false;
    }
    return false;
}

bool RendererWrapper::ExternalNativeEvent(const QByteArray& event_type, void* message, long* result)
{
    return nativeEvent(event_type, message, result);
}

void RendererWrapper::mousePressEvent(QMouseEvent* e)
{
    if (e->buttons().testFlag(Qt::MouseButton::LeftButton))
    { }
}

RendererWrapper::~RendererWrapper()
{
}
