#ifndef APPROXGUIMENUBAR_H
#define APPROXGUIMENUBAR_H

#include <QWidget>

namespace ApproxGUI
{
    class ApproxGUIMenuBar : public QWidget
    {
        Q_OBJECT

    public:
        ApproxGUIMenuBar(QWidget *parent = 0);
        ~ApproxGUIMenuBar();
        void addWidget(QWidget*);
    private:

    };
}
#endif // APPROXGUIMENUBAR_H
