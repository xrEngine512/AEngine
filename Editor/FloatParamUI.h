#ifndef FLOATPARAMUI_H
#define FLOATPARAMUI_H

#include <QWidget>
#include "ui_FloatParamUI.h"

namespace ShaderSystem{
	union floatVariant;
}

namespace ASL{
	struct ShaderParamInfo;
}

class FloatParamUI : public QWidget
{
	Q_OBJECT

public:
	FloatParamUI(QWidget *parent, const ASL::ShaderParamInfo& param);
	~FloatParamUI();
signals:
	void valueChanged(const ShaderSystem::floatVariant&);
private:
	Ui::FloatParamUI ui;
};

#endif // FLOATPARAMUI_H
