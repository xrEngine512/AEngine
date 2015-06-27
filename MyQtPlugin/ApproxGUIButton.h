#ifndef MYQT_H
#define MYQT_H

#include <QtWidgets/qpushbutton.h>

class ApproxGUIButton : public QPushButton
{
    Q_OBJECT
        Q_PROPERTY(double dotSize READ GetDotSize WRITE SetDotSize)
        Q_PROPERTY(double shift READ GetShift WRITE SetShift)
        Q_PROPERTY(double opacity READ GetOpacity WRITE SetOpacity)
public:
    __declspec(dllexport) ApproxGUIButton(QWidget *parent = 0);
    ~ApproxGUIButton();
    void SetDotSize(double);
    void SetShift(double);
    void SetOpacity(double);
    double GetDotSize()const;
    double GetShift()const;
    double GetOpacity()const;
private:
    void paintEvent(QPaintEvent*)override;
    void changeEvent(QEvent* e)override;

    double m_dotsSizeHalf, m_shift;
    double m_opacity;
};

#endif // MYQT_H
