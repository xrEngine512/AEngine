#include "ApproxGUIMenuButton.h"
#include <qpainter.h>
#include <qevent.h>
#include <QMenu>

namespace ApproxGUI
{
    ApproxGUIMenuButton::ApproxGUIMenuButton(QWidget *parent)
        : QPushButton(parent), m_dotsSizeHalf(3), m_shift(10), m_opacity(1)
    {
        resize(100, 23);
        setAttribute(Qt::WA_TranslucentBackground);
    }

    ApproxGUIMenuButton::~ApproxGUIMenuButton()
    {

    }

    void ApproxGUIMenuButton::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);     
        painter.setRenderHint(QPainter::Antialiasing);
       
        QPoint points[4] =
        {
            QPoint(m_dotsSizeHalf, m_dotsSizeHalf), 
			QPoint(width() - m_shift - m_dotsSizeHalf, m_dotsSizeHalf), 
			QPoint(width() - m_dotsSizeHalf, height() - m_dotsSizeHalf), 
			QPoint(m_shift + m_dotsSizeHalf, height() - m_dotsSizeHalf)
        };
        QPainterPath path;
        path.moveTo(points[0]);
        path.lineTo(points[1]);
        path.lineTo(points[2]);
        path.lineTo(points[3]);
        path.lineTo(points[0]);
		
		if (menu())
		{
			painter.setPen(QPen(QColor(200, 200, 200, m_opacity * 255), m_dotsSizeHalf * 2));
			painter.drawPoint(points[2]);
		}

        if (isEnabled())
        {
            if (underMouse())
            {
                if (isDown())
                {
                    painter.fillPath(path, QColor(10, 10, 10));
                }
                else
                {
                    painter.setPen(QPen(QColor(200, 200, 200), m_dotsSizeHalf * 2));
                    painter.drawPoints(points, 4);
                }
            }
        }
        else
        {
            painter.fillPath(path, QColor(100, 100, 100, m_opacity * 255));
        }
        painter.setPen(QPen(QColor(200, 200, 200, m_opacity * 255), 1));
        painter.drawPath(path);
        painter.drawText(0, 0, width(), height(), Qt::AlignCenter, text());
		
    }

	void ApproxGUIMenuButton::SetDotSize(int arg)
    {
        m_dotsSizeHalf = arg / 2;
    }

	void ApproxGUIMenuButton::SetShift(int shift)
    {
        m_shift = shift;
    }

	void ApproxGUIMenuButton::setMenu(QMenu* menu)
	{
		QPushButton::setMenu(menu);
		class Filter : public QObject
		{
		public:
			bool eventFilter(QObject * obj, QEvent *event)override
			{
				ApproxGUIMenuButton* parentButton = dynamic_cast<ApproxGUIMenuButton*>(obj->parent());
				if (!parentButton)
					return false;

				QMenu* menu = dynamic_cast<QMenu*>(obj);
				if (!menu)
					return false;

				if (event->type() == QEvent::Show && obj == parentButton->menu())
				{
					QPoint pos = menu->pos();

					pos.setX(pos.x() + parentButton->width() - parentButton->DotSizeHalf());
					pos.setY(pos.y() - parentButton->DotSizeHalf());
					parentButton->menu()->move(pos);
					return true;
				}
				return false;
			}
		};
		menu->installEventFilter(new Filter);
	}

	void ApproxGUIMenuButton::changeEvent(QEvent* e)
    {
        if (e->type() == QEvent::EnabledChange)
        {
            if (isEnabled())
                m_opacity = 1;
            else
                m_opacity = 0.5;
            e->accept();
        }
    }
}