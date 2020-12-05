#include <QApplication>
#include <assimp/Importer.hpp>
#include "LoadEngineWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Assimp::Importer importer;

    LoadEngineWidget w;

    w.show();
    return (a.exec());
}
