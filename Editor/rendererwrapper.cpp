#include "rendererwrapper.h"

#ifdef None
#undef None
#endif

#include <QMouseEvent>


RendererWrapper::RendererWrapper(QWidget *parent) : QWidget(parent), m_input(nullptr)
{
    ui.setupUi(this);
    sys = GetSystem();
    setAttribute(Qt::WA_NoSystemBackground, true);
}

bool RendererWrapper::Initialize()
{
    if(!sys->initialize(winId(), false, 1280, 720, x(), y()))
        return false;
    m_input = GetInput();
    this->resize(1280, 720);
    return true;
}

void RendererWrapper::Run()
{
    return sys->Run();
}

void RendererWrapper::Shutdown()
{
    return sys->shutdown();
}

void RendererWrapper::GetCameraPos(float &x, float &y, float &z)
{
    sys->GetRendererEx()->get_scene()->GetCameraPos(x, y, z);
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
        //sys->GetRendererEx()->SetPosition(global_pos.x(), global_pos.y());
    }
}

ISystem* RendererWrapper::NativeInterface()
{
    return sys;
}

RenderObjectPromise RendererWrapper::LoadModel(const std::string &objFilename, const ShaderDesc &type)
{
    return std::move(sys->GetRendererEx()->get_scene()->AddRenderObject(objFilename, type));
}

void RendererWrapper::moveEvent(QMoveEvent*)
{    
    PosUpdate();
}

bool RendererWrapper::ExternalNativeEvent(const QByteArray& event_type, void* message, long* result)
{
    return nativeEvent(event_type, message, result);
}

RendererWrapper::~RendererWrapper()
{
}

void RendererWrapper::mousePressEvent(QMouseEvent *event) {
    m_input->onMouseDown(event->button());
    QWidget::mouseReleaseEvent(event);
}

void RendererWrapper::mouseReleaseEvent(QMouseEvent *event) {
    m_input->onMouseUp(event->button());
    QWidget::mouseReleaseEvent(event);
}

void RendererWrapper::keyPressEvent(QKeyEvent *event) {
    m_input->onKeyDown(event->key());
    QWidget::keyPressEvent(event);
}

void RendererWrapper::keyReleaseEvent(QKeyEvent *event) {
    m_input->onKeyUp(event->key());
    QWidget::keyReleaseEvent(event);
}
