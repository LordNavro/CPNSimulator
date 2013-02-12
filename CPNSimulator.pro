#-------------------------------------------------
#
# Project created by QtCreator 2013-02-06T14:49:05
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPNSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    place.cpp \
    transition.cpp \
    arc.cpp \
    cpnet.cpp \
    sheettabwidget.cpp \
    cpneteditor.cpp \
    arcform.cpp \
    placeform.cpp \
    transitionform.cpp \
    cpnetform.cpp \
    cpnetscene.cpp \
    placeitem.cpp \
    transitionitem.cpp \
    arcitem.cpp \
    inscriptionedit.cpp \
    inscriptionhighlighter.cpp

HEADERS  += mainwindow.h \
    place.h \
    transition.h \
    arc.h \
    cpnet.h \
    sheettabwidget.h \
    cpneteditor.h \
    arcform.h \
    placeform.h \
    transitionform.h \
    cpnetform.h \
    cpnetscene.h \
    placeitem.h \
    transitionitem.h \
    arcitem.h \
    inscriptionedit.h \
    inscriptionhighlighter.h

RESOURCES += \
    icons.qrc
