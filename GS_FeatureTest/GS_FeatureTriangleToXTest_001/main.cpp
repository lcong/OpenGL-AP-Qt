#include <QApplication>
#include "QtFunctionWidget.h"
#include "CoreFunctionWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CoreFunctionWidget w1;

    QtFunctionWidget w2;

    w1.setWindowTitle(QObject::tr("GS_FeaturePointsToXTest_001"));
//    w2.setWindowTitle(QObject::tr("QtFunctionWidget"));
    w1.show();
//    w2.show();
    return (a.exec());
}
