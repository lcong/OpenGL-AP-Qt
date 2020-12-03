#include "GlWidget.h"
#include <QApplication>
#include <assimp/Importer.hpp>

#include "LoadEngineWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlWidget w;

//    Assimp::Importer importer;

    LoadEngineWidget w2;
    GlWidget w1;

    w1.show();
    w2.show();
//    w.show();
    return (a.exec());
}
