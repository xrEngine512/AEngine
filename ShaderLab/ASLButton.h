#ifndef ASLBUTTON_H
#define ASLBUTTON_H

#include <QPushButton>

class ASLButton : public QPushButton
{
    Q_OBJECT

public:
    ASLButton(QWidget *parent = 0);
    ~ASLButton();

private:
    void paintEvent(QPaintEvent*)override;
private slots:

};

#endif // ASLBUTTON_H
