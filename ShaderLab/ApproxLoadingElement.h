#ifndef APPROXLOADINGELEMENT_H
#define APPROXLOADINGELEMENT_H

#include <QObject>
#include <QGraphicsItem>

class QPropertyAnimation;


class ApproxLoadingElement : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(double phase READ getPhase WRITE setPhase)
    Q_INTERFACES(QGraphicsItem)
public:
    ApproxLoadingElement(QObject *parent);
    ~ApproxLoadingElement();
    double getPhase()const;
    void setPhase(double);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
private:
     QPropertyAnimation *anim;
     double m_phase;
};

#endif // APPROXLOADINGELEMENT_H
