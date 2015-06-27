#ifndef MATERIALVARPOINT_H
#define MATERIALVARPOINT_H

#include <QLabel>

class MaterialVarPoint : public QLabel
{
	typedef QLabel BaseClass;
	Q_OBJECT

public:
	MaterialVarPoint(QWidget *parent);
	~MaterialVarPoint();

signals:
	void linkAttempt(QWidget* sender, const QPoint& mouseGlobalPos);

private:
	void enterEvent(QEvent*)override;
	void leaveEvent(QEvent*)override;
	void mousePressEvent(QMouseEvent*)override;
	void mouseReleaseEvent(QMouseEvent*)override;
	void mouseMoveEvent(QMouseEvent*)override;

	
};

#endif // MATERIALVARPOINT_H
