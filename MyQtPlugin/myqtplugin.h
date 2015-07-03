#ifndef MYQTPLUGIN_H
#define MYQTPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>

class MyQtPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "myqtplugin.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    MyQtPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool initialized;
};

#endif // MYQTPLUGIN_H