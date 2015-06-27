#include "ApproxLoadingElement.h"
#include <qpropertyanimation.h>
#include <qpainter.h>

static const double len = 100;
static const double amplitude = 30;

ApproxLoadingElement::ApproxLoadingElement(QObject *parent)
    : QObject(parent), m_phase(0)
{
    anim = new QPropertyAnimation(this, "phase");
    anim->setDuration(10000);
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->setStartValue(0);
    anim->setEndValue(50);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

double ApproxLoadingElement::getPhase() const
{
    return m_phase;
}

void ApproxLoadingElement::setPhase(double phase)
{
    m_phase = phase;
}

void ApproxLoadingElement::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* )
{
    static double u(8);
    static bool t(true);

    if (t)
    {
        u += .2;
        if (u >= 30)
            t = false;
    }
    if (!t)
    {
        u -= .2;
        if (u < 5)
            t = true;
    }
    QPainterPath* m_path = new QPainterPath;
    QPen pen1(QBrush(QColor(qRgb(160, 160, 160))), 2);
    QPen pen2(QBrush(QColor(qRgb(0, 0, 0))), 1);
    QPen pen3(QBrush(QColor(qRgb(120, 120, 120))), 5);
    m_path->moveTo(x(), y());
    // m_phase += 0.1;    

    for (double x = 0; x < len; x += len / u)
    {
        m_path->lineTo(x, amplitude*sin(0.2*x - m_phase));
    }
    painter->setPen(pen1);
    painter->drawPath(*m_path);
    delete m_path;
    painter->setPen(pen3);
    for (double x = 0; x < len; x += len / u)
    {
        painter->drawPoint(x, amplitude*sin(0.2*x - m_phase));
    }
    m_path = new QPainterPath;
    m_path->moveTo(x(), y());
    painter->setPen(pen2);
    for (double x = 0; x < len; x += len / 100)
    {
        m_path->lineTo(x, amplitude*sin(0.2*x - m_phase));
    }
    painter->drawPath(*m_path);
    delete m_path;
    painter->setPen(pen1);
    painter->drawRect(boundingRect());
}

ApproxLoadingElement::~ApproxLoadingElement()
{

}
