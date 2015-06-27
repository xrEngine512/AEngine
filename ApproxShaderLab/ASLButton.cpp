#include "ASLButton.h"
#include <qpainter.h>

ASLButton::ASLButton(QWidget *parent)
    : QPushButton(parent)
{
    resize(100, 23);
}



ASLButton::~ASLButton()
{

}

void ASLButton::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    QRect br(0, 0, width(), height());
    
    QPoint points[4] =
    {
        QPoint(0, 0), QPoint(width(), 0), QPoint(width(), height()), QPoint(0, height())
    };
   
    if (underMouse())
    {
        if (isDown())
        {
            painter.fillRect(br, QColor(10, 10, 10));
            
        }
        else
        {
            painter.setPen(QPen(QColor(200, 200, 200), 10));
            painter.drawPoints(points, 4);            
        }
    }
    painter.setPen(QPen(QColor(200, 200, 200), 2));
    painter.drawRect(0, 0, width(), height());
    painter.drawText(0, 0, width(), height(), Qt::AlignCenter, text());
    
}