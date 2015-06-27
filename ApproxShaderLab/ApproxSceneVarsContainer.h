#ifndef APPROXSCENEVARS_H
#define APPROXSCENEVARS_H

#include <QFrame>
#include <vector>
#include "ScenePropertyVisualInfo.h"
using namespace std;
class ApproxSceneVar;

class ApproxSceneVarsContainer : public QFrame
{
    Q_OBJECT

public:
    ApproxSceneVarsContainer(QWidget *parent, std::vector<ScenePropertyVisualInfo>const&);
    ~ApproxSceneVarsContainer();
signals:
	void linkAttempt(QWidget* sender, const QPoint& mouseGlobalPosEnd);
private:
    vector < ApproxSceneVar* >  m_vars;
};

#endif // APPROXSCENEVARS_H
