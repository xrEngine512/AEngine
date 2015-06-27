#ifndef Button_H
#define Button_H

#include <QPushButton>

class QLabel;
class QPixmap;
class ButtonIconSet;

class Button : public QPushButton
{
    Q_OBJECT

public:
	Button(QString ButtonTypeName, QWidget *parent = 0);
    ~Button();

private:
	QLabel* m_lblIcon;
	ButtonIconSet* m_IconSet;
	void enterEvent(QEvent*)override;
	void leaveEvent(QEvent*)override;
	void mousePressEvent(QMouseEvent*)override;
	void mouseReleaseEvent(QMouseEvent*)override;
};

#endif // Button_H
