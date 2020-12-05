QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GlWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    GlWidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    image/image.qrc \
    shader/shader.qrc \
    objects/objects.qrc

DISTFILES += \
    image/awesomeface.png \
    image/container.jpg \
    image/container2.png \
    image/container2_specular.png \
    image/wall.jpg \
    shader/basic_lighting.frag \
    shader/basic_lighting.vert \
    shader/colortriangle.frag \
    shader/colortriangle.vert \
    shader/lamp.frag \
    shader/lamp.vert \
    shader/lighting_maps.frag \
    shader/lighting_maps.vert \
    shader/model_loading.fs \
    shader/model_loading.vs \
    shader/textures.frag \
    shader/textures.vert \
    shader/triangle.frag \
    shader/triangle.vert




win32: LIBS += -L$$PWD/../../../Qt/5.15.2/mingw81_64/thirdparty/assimp/lib/ -llibassimp.dll

INCLUDEPATH += $$PWD/../../../Qt/5.15.2/mingw81_64/thirdparty/assimp/include
DEPENDPATH += $$PWD/../../../Qt/5.15.2/mingw81_64/thirdparty/assimp/include
