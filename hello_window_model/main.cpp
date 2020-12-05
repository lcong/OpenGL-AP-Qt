#include "GlWidget.h"
#include <QApplication>
#include <assimp/Importer.hpp>

#include "LoadEngineWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Assimp::Importer importer;

    GlWidget w1;
    LoadEngineWidget w2;

    w1.show();
    w2.show();
//    w.show();
    return (a.exec());
}
