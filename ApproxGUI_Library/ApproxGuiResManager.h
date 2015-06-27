#ifndef APPROXGUIRESMANAGER_H
#define APPROXGUIRESMANAGER_H
#include <map>
#include <QObject>

class QPixmap;
class QString;

class ApproxGuiResManager : public QObject
{
    Q_OBJECT

    std::map<QString, QPixmap*> m_refs;
public:
    
    QPixmap* GetPic(QString);
    ~ApproxGuiResManager();
    static ApproxGuiResManager* Instance();
    void ReleaseResources();
private:
    ApproxGuiResManager(QObject *parent);
};

extern ApproxGuiResManager* g_ResManager;

#endif // APPROXGUIRESMANAGER_H
