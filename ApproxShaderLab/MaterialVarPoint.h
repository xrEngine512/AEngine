#ifndef MATERIALVARPOINT_H
#define MATERIALVARPOINT_H

#include <QLabel>

namespace ASL
{
	//class MaterialVar;

	class MaterialVarPoint : public QLabel
	{
		typedef QLabel BaseClass;
		Q_OBJECT

	public:
		MaterialVarPoint(QWidget *parent);
		~MaterialVarPoint();

	signals:
		void linkAttempt(const QPoint& mouseGlobalPos);

	private:
		void enterEvent(QEvent*)override;
		void leaveEvent(QEvent*)override;
		void mousePressEvent(QMouseEvent*)override;
		void mouseReleaseEvent(QMouseEvent*)override;
		void mouseMoveEvent(QMouseEvent*)override;

		//MaterialVar* m_Var;
	};
}
#endif // MATERIALVARPOINT_H
