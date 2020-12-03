#include <QApplication>
#include "CoreFunctionWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CoreFunctionWidget w1;

    w1.setWindowTitle(QObject::tr("CoreFunction"));
    w1.show();

    return (a.exec());
}
