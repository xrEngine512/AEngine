#ifndef APPROXSCENEVAR_H
#define APPROXSCENEVAR_H

#include <QWidget>
#include "ScenePropertyVisualInfo.h"

__interface ISceneProperty;
class QLabel;
class MaterialVarPoint;
class ScenePropertyVisualInfo;

class ApproxSceneVar : public QWidget
{
    Q_OBJECT

    ScenePropertyVisualInfo m_info;
public:
    ApproxSceneVar(QWidget *parent, const ScenePropertyVisualInfo& info);
    ~ApproxSceneVar();
	const MaterialVarPoint* Point()const;
private:
    QLabel* m_flag, *m_name;
	MaterialVarPoint *m_point;
};

#endif // APPROXSCENEVAR_H
