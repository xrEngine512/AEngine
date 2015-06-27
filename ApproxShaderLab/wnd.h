#ifndef WND_H
#define WND_H

#include <QWidget>
#include "ui_wnd.h"

class wnd : public QWidget
{
    Q_OBJECT

public:
    wnd(QWidget *parent = 0);
    ~wnd();

    Ui::wnd ui;
private:
    
};

#endif // WND_H
