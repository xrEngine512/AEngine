#ifndef APPROXSIZEGRIP_H
#define APPROXSIZEGRIP_H

#include <QWidget>

namespace ApproxGUI
{
	enum Position{ TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
	class SizeGrip : public QWidget
	{
		typedef QWidget BaseClass;
		Q_OBJECT
	public:
		SizeGrip(QWidget *parent);
		void snapTo(Position);
		~SizeGrip();
	public slots:
		void snap();
	protected:
		void moveEvent(QMoveEvent*)override;
		void mouseMoveEvent(QMouseEvent *)override final;
		void mousePressEvent(QMouseEvent *)override final;
		void mouseReleaseEvent(QMouseEvent *)override final;
		void enterEvent(QEvent*)override final;
		void leaveEvent(QEvent*)override final;
	private:
		Position GetPosition();

		bool m_moved;
		Position m_lastPosition;
		QPoint m_Prev;
		bool m_dragging;
	};
}
#endif // APPROXSIZEGRIP_H
