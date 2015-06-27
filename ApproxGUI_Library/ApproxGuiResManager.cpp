#include "ApproxGuiResManager.h"
#include <qstring.h>
#include <qpixmap.h>
#include <QLabel>

ApproxGuiResManager::ApproxGuiResManager(QObject *parent)
    : QObject(parent)
{

}

ApproxGuiResManager*  g_ResManager = ApproxGuiResManager::Instance();

QPixmap* ApproxGuiResManager::GetPic(QString str)
{
    auto res = m_refs.find(str);
    if (res != m_refs.end())
    {
        return res->second;
    }
    QPixmap* pic = new QPixmap(str);
    m_refs.insert(std::pair <QString, QPixmap*>(str, pic));
    return pic;
}

ApproxGuiResManager::~ApproxGuiResManager()
{
    ReleaseResources();
}

ApproxGuiResManager* ApproxGuiResManager::Instance()
{
    static ApproxGuiResManager object(nullptr);
    return &object;
}

void ApproxGuiResManager::ReleaseResources()
{
    for(auto ref: m_refs)
    {
        delete ref.second;
    }
    m_refs.clear();
}