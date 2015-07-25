#ifndef RENDERERWRAPPER_H
#define RENDERERWRAPPER_H

#include <QWidget>
#include "ui_rendererwrapper.h"
#include <ISystem.h>
#include <IInternalInput.h>

class RendererWrapper : public QWidget
{
    Q_OBJECT

public:
    RendererWrapper(QWidget *parent = nullptr);
    ~RendererWrapper();
    bool Initialize();
    void Run();
    void Shutdown();
	IExternalRenderObject* LoadModel(char* objFilename, const ShaderDesc& type);
    void GetCameraPos(float &x, float &y, float &z);
    void PosUpdate();
    ISystem* NativeInterface();
    bool ExternalNativeEvent(const QByteArray&, void*, long*);
private:
    void moveEvent(QMoveEvent*)override;
    bool nativeEvent(const QByteArray&, void*, long*)override;
    
    void mousePressEvent(QMouseEvent*)override;
    
    Ui::RendererWrapper ui;
    ISystem *sys;
    IInternalInput *m_input;
};

#endif // RENDERERWRAPPER_H
