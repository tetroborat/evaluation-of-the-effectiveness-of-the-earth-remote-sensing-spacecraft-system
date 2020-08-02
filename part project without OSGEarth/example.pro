#-------------------------------------------------
#
# Project created by QtCreator 2018-09-11T20:41:00
#
#-------------------------------------------------

QT       += core gui sql
#CONFIG += qwt
#CONFIG     += QwtSvg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app

#INCLUDEPATH += C:/Trolltech/OSG/include
#LIBS += -LC:/Trolltech/OSG/bin -losgd -lOpenThreadsd -losgDBd -losgGAd -losgQtd -losgViewerd -losgWidgetd -losgSimd -losgTextd
#LIBS += -LC:/Trolltech/OSG/bin -losgEarthd -losgEarthUtild -losgEarthFeaturesd -losgEarthAnnotationd -losgEarthSymbologyd
#INCLUDEPATH += C:/Trolltech/qwt-6.1.3/src
#LIBS += -LC:/Trolltech/qwt-6.1.3/lib -lqwtd

INCLUDEPATH += D:/Qt/qwt-6.1.4/src
LIBS += -LD:/Qt/qwt-6.1.4/lib -lqwtd
INCLUDEPATH += D:/proc/OSG/include
LIBS += -LD:/proc/OSG/bin -losgd -lOpenThreadsd -losgDBd -losgGAd -losgQtd -losgViewerd -losgWidgetd -losgSimd -losgTextd
LIBS += -LD:/proc/OSG/bin -losgEarthd -losgEarthUtild -losgEarthFeaturesd -losgEarthAnnotationd -losgEarthSymbologyd

SOURCES += main.cpp\
        mainwindow.cpp \
    2d.cpp \
    3d.cpp \
    input.cpp \
    listsc.cpp \
    output.cpp \
    spacecraft.cpp \
    convert.cpp \
    manager.cpp \
    calculation.cpp \
    gui3d/osgviewerQt.cpp \
    gui3d/scene3d.cpp \
    listregion.cpp \
    model_viewing_areaSC.cpp \
    listgcc.cpp

HEADERS  += mainwindow.h \
    2d.h \
    3d.h \
    input.h \
    listsc.h \
    output.h \
    spacecraft.h \
    convert.h \
    manager.h \
    calculation.h \
    gui3d/osgviewerQt.h \
    gui3d/scene3d.h \
    constants.h \
    listregion.h \
    model_viewing_areaSC.h \
    listgcc.h

FORMS    += mainwindow.ui \
    2d.ui \
    3d.ui \
    input.ui \
    output.ui \
    listsc.ui \
    listregion.ui \
    listgcc.ui

