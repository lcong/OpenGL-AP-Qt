#include "GlWidget.h"
#include <QApplication>
#include <assimp/Importer.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GlWidget w1;

    w1.show();
    return (a.exec());
}
