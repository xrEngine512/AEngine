#ifndef APPROXGUIMENUBUTTON_H
#define APPROXGUIMENUBUTTON_H

#include <QPushButton>

namespace ApproxGUI
{
    class ApproxGUIMenuButton : public QPushButton
    {
        Q_OBJECT

    public:
	    explicit ApproxGUIMenuButton(QWidget *parent = 0);
        ~ApproxGUIMenuButton();
        void paintEvent(QPaintEvent*)override;
		void SetDotSize(int);
        void SetShift(int);
		int DotSizeHalf(){ return m_dotsSizeHalf; }
		void setMenu(QMenu*);
    private:
        void changeEvent(QEvent* e)override;
		int m_dotsSizeHalf, m_shift;
		qreal m_opacity;
    };
}
#endif // APPROXGUIMENUBUTTON_H
