#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createToolBars();
    createMenuBars();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    actionNew = new QAction(QIcon(":icons/icons/new.ico"), tr("New net"), this);
    actionSave = new QAction(QIcon(":icons/icons/save.ico"), tr("Save net"), this);
    actionLoad = new QAction(QIcon(":icons/icons/load.ico"), tr("Load net"), this);

    actionPlace = new QAction(QIcon(":icons/icons/place.ico"), tr("New place"), this);
    actionTransition = new QAction(QIcon(":icons/icons/transition.ico"), tr("New transition"), this);
    actionArc = new QAction(QIcon(":icons/icons/arc.ico"), tr("New arc"), this);
    actionDelete = new QAction(QIcon(":icons/icons/delete.ico"), tr("Delete object"), this);

    actionAbout = new QAction(QIcon(":icons/icons/about.ico"), tr("About program"), this);
}

void MainWindow::createToolBars()
{
    toolBarFile = addToolBar(tr("File options"));
    toolBarSimulation = addToolBar(tr("Simulation options"));
    toolBarTools = addToolBar(tr("Tool options"));

    toolBarFile->addAction(actionNew);
    toolBarFile->addAction(actionSave);
    toolBarFile->addAction(actionLoad);

    toolBarTools->addAction(actionPlace);
    toolBarTools->addAction(actionTransition);
    toolBarTools->addAction(actionArc);
    toolBarTools->addAction(actionDelete);

}

void MainWindow::createMenuBars()
{
    menuFile = menuBar()->addMenu(QIcon(":icons/icons/file.ico"), tr("File"));
    menuFile->addAction(actionNew);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionLoad);
}
