#ifndef RENDERERWRAPPER_H
#define RENDERERWRAPPER_H

#include <QWidget>
#include "ui_rendererwrapper.h"
#include <ISystem.h>
#include <include/IInternalInput.h>

class RendererWrapper : public QWidget
{
    Q_OBJECT

public:
    RendererWrapper(QWidget *parent = nullptr);
    ~RendererWrapper();
    bool Initialize();
    void Run();
    void Shutdown();
	RenderObjectPromise LoadModel(const std::string &objFilename, const ShaderDesc &type);
    void GetCameraPos(float &x, float &y, float &z);
    void PosUpdate();
    ISystem* NativeInterface();
    bool ExternalNativeEvent(const QByteArray&, void*, long*);
private:
    void moveEvent(QMoveEvent*)override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
    
    Ui::RendererWrapper ui;
    ISystem *sys;
    IInternalInput *m_input;
};

#endif // RENDERERWRAPPER_H
