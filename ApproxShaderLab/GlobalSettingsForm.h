#ifndef GLOBALSETTINGSFORM_H
#define GLOBALSETTINGSFORM_H

#include <QWidget>
#include "ui_GlobalSettingsForm.h"

class GlobalSettingsForm : public QWidget
{
	Q_OBJECT

public:
	GlobalSettingsForm(QWidget *parent = 0);
	~GlobalSettingsForm();

private:
	Ui::GlobalSettingsForm ui;
	QFrame* m_background;

	void resizeEvent(QResizeEvent*)override;
};

#endif // GLOBALSETTINGSFORM_H
