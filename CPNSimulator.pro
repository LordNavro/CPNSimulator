#-------------------------------------------------
#
# Project created by QtCreator 2013-02-06T14:49:05
#
#-------------------------------------------------

QT      += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPNSimulator
TEMPLATE = app

include(flex.pri)
include(bison.pri)

FLEXSOURCES += scanner.l
BISONSOURCES += parser.y

SOURCES += main.cpp\
    mainwindow.cpp \
    place.cpp \
    transition.cpp \
    arc.cpp \
    cpnet.cpp \
    cpneteditor.cpp \
    arcform.cpp \
    placeform.cpp \
    transitionform.cpp \
    cpnetform.cpp \
    cpnetscene.cpp \
    inscriptionedit.cpp \
    inscriptionhighlighter.cpp \
    compiler.cpp \
    symboltable.cpp \
    cpnetsimulator.cpp \
    editorarcitem.cpp \
    editortransitionitem.cpp \
    editorplaceitem.cpp \
    arcitem.cpp \
    placeitem.cpp \
    transitionitem.cpp

HEADERS  += mainwindow.h \
    place.h \
    transition.h \
    arc.h \
    cpnet.h \
    cpneteditor.h \
    arcform.h \
    placeform.h \
    transitionform.h \
    cpnetform.h \
    cpnetscene.h \
    inscriptionedit.h \
    inscriptionhighlighter.h \
    compiler.h \
    symboltable.h \
    cpnetsimulator.h \
    editortransitionitem.h \
    editorplaceitem.h \
    editorarcitem.h \
    arcitem.h \
    placeitem.h \
    transitionitem.h

RESOURCES += \
    icons.qrc
