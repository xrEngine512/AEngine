#ifndef MATERIALVARPOINT_H
#define MATERIALVARPOINT_H

#include <QLabel>

namespace ASL
{
	//class MaterialVar;

	class LinkingPoint : public QLabel
	{
		typedef QLabel BaseClass;
		Q_OBJECT

	public:
		LinkingPoint(QWidget *parent);
		~LinkingPoint();

	signals:
		void linkAttempt(const QPoint& mouseGlobalPos);
	public slots:
		void setInUse();
		void setInFocus();
		void setNotInUse();

		void addRef();
		void deleteRef();
	private:
		void enterEvent(QEvent*)override;
		void leaveEvent(QEvent*)override;
		void mousePressEvent(QMouseEvent*)override;
		void mouseReleaseEvent(QMouseEvent*)override;
		void mouseMoveEvent(QMouseEvent*)override;

		int m_refCount;
		//MaterialVar* m_Var;
	};
}
#endif // MATERIALVARPOINT_H
