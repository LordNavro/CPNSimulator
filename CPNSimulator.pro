#-------------------------------------------------
#
# Project created by QtCreator 2013-02-06T14:49:05
#
#-------------------------------------------------

QT      += core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPNSimulator
TEMPLATE = app

CONFIG += exceptions

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
    transitionitem.cpp \
    simulatorarcitem.cpp \
    simulatorplaceitem.cpp \
    simulatortransitionitem.cpp \
    editorscene.cpp \
    simulatorscene.cpp \
    interpret.cpp \
    computer.cpp \
    analyzer.cpp \
    statespaceexplorer.cpp \
    assistant.cpp

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
    transitionitem.h \
    simulatorarcitem.h \
    simulatorplaceitem.h \
    simulatortransitionitem.h \
    editorscene.h \
    simulatorscene.h \
    interpret.h \
    computer.h \
    analyzer.h \
    statespaceexplorer.h \
    assistant.h

RESOURCES += \
    icons.qrc \
    schema.qrc

OTHER_FILES += \
    doc/index.html \
    doc/editor.html \
    doc/cpnsimulator.qhp \
    doc/cpnsimulator.qhcp \
    doc/about.txt \
    doc/simulator.html \
    doc/analysis.html \
    LICENCE.txt \
    INSTALL.txt
