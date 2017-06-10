#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include "Editor.h"
#include <boost/locale/generator.hpp>

using namespace boost::locale;

int main(int argc, char *argv[])
{
    generator gen;
    // Create locale generator
    std::locale::global(gen(""));
    // "" - the system default locale, set
    // it globally

	QApplication a(argc, argv);
    Editor w;
    w.show();
    return a.exec();
}
