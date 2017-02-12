#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include "Editor.h"
#include <thread>

void s()
{
	std::this_thread::sleep_for(std::chrono::seconds(12));
}

int main(int argc, char *argv[])
{
	s();
	QApplication a(argc, argv);
    Editor w;
    w.show();
    return a.exec();
}
