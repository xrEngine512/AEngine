#include "ApproxGUIButton.h"
#include <qpainter.h>
#include <qevent.h>

ApproxGUIButton::ApproxGUIButton(QWidget *parent)
    : QPushButton(parent), m_dotsSizeHalf(3), m_shift(10), m_opacity(1)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

ApproxGUIButton::~ApproxGUIButton()
{

}

void ApproxGUIButton::SetDotSize(double size)
{
    m_dotsSizeHalf = size / 2;
}

void ApproxGUIButton::SetShift(double shift)
{
    m_shift = shift;
}

void ApproxGUIButton::SetOpacity(double op)
{
    m_opacity = op;
}

double ApproxGUIButton::GetDotSize() const
{
    return m_dotsSizeHalf * 2;
}

double ApproxGUIButton::GetShift() const
{
    return m_shift;
}

double ApproxGUIButton::GetOpacity() const
{
    return m_opacity;
}

void ApproxGUIButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);


    QPoint points[4] =
    {
        QPoint(m_dotsSizeHalf, m_dotsSizeHalf), QPoint(width() - m_shift - m_dotsSizeHalf, m_dotsSizeHalf), QPoint(width() - m_dotsSizeHalf, height() - m_dotsSizeHalf), QPoint(m_shift + m_dotsSizeHalf, height() - m_dotsSizeHalf)
    };
    QPainterPath path;
    path.moveTo(points[0]);
    path.lineTo(points[1]);
    path.lineTo(points[2]);
    path.lineTo(points[3]);
    path.lineTo(points[0]);

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

void ApproxGUIButton::changeEvent(QEvent* e)
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