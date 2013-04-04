#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createToolBars();
    createMenuBars();

    tabWidget = new SheetTabWidget(this);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabCloseRequest(int)));
    this->setCentralWidget(tabWidget);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int));

    setWindowTitle("CPNSimulator");
    setWindowIcon(QIcon(style()->standardIcon(QStyle::SP_CommandLink)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    actionNew = new QAction(QIcon(style()->standardIcon(QStyle::SP_DialogResetButton)), tr("New net"), this);
    actionSave = new QAction(QIcon(style()->standardIcon(QStyle::SP_DialogSaveButton)), tr("Save net"), this);
    actionSaveAs = new QAction(QIcon(QIcon(":icons/icons/save.ico")), tr("Save net as"), this);
    actionLoad = new QAction(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)), tr("Load net"), this);
    actionClose = new QAction(QIcon(style()->standardIcon(QStyle::SP_DialogCloseButton)), tr("Close net"), this);

    connect(actionNew, SIGNAL(triggered()), this, SLOT(slotNew()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(slotSave()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(slotLoad()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(slotClose()));

    actionGroupTool = new QActionGroup(this);
    actionSelect = new QAction(QIcon(":icons/icons/cursor.ico"), tr("Select"), actionGroupTool);
    actionPlace = new QAction(QIcon(":icons/icons/place.ico"), tr("New place"), actionGroupTool);
    actionTransition = new QAction(QIcon(":icons/icons/transition.ico"), tr("New transition"), actionGroupTool);
    actionArc = new QAction(QIcon(":icons/icons/arc.ico"), tr("New arc"), actionGroupTool);
    actionDelete = new QAction(QIcon(":icons/icons/delete.ico"), tr("Delete object"), actionGroupTool);
    actionSelect->setCheckable(true);
    actionPlace->setCheckable(true);
    actionTransition->setCheckable(true);
    actionArc->setCheckable(true);
    actionDelete->setCheckable(true);
    actionSelect->setChecked(true);

    connect(actionSelect, SIGNAL(triggered()), this, SLOT(slotSelect()));
    connect(actionPlace, SIGNAL(triggered()), this, SLOT(slotPlace()));
    connect(actionTransition, SIGNAL(triggered()), this, SLOT(slotTransition()));
    connect(actionArc, SIGNAL(triggered()), this, SLOT(slotArc()));
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(slotDelete()));

    actionAbout = new QAction(QIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation)), tr("About program"), this);

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

void MainWindow::createToolBars()
{
    toolBarFile = addToolBar(tr("File options"));
    toolBarSimulation = addToolBar(tr("Simulation options"));
    toolBarTools = addToolBar(tr("Tool options"));

    toolBarFile->addAction(actionNew);
    toolBarFile->addAction(actionLoad);
    toolBarFile->addAction(actionSave);
    toolBarFile->addAction(actionSaveAs);
    toolBarFile->addAction(actionClose);

    toolBarTools->addAction(actionSelect);
    toolBarTools->addAction(actionPlace);
    toolBarTools->addAction(actionTransition);
    toolBarTools->addAction(actionArc);
    toolBarTools->addAction(actionDelete);

}

void MainWindow::createMenuBars()
{
    menuFile = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_FileIcon)), tr("File"));
    menuTool = menuBar()->addMenu(QIcon(":/icons/icons/hammer.ico"), tr("Tool"));
    menuAbout = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation)), tr("About"));

    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionClose);

    menuTool->addAction(actionSelect);
    menuTool->addAction(actionPlace);
    menuTool->addAction(actionTransition);
    menuTool->addAction(actionArc);
    menuTool->addAction(actionDelete);

    menuAbout->addAction(actionAbout);

}

void MainWindow::slotNew()
{
    CPNetEditor *e = new CPNetEditor(this);
    tabWidget->addTab(e, "New net");
    tabWidget->setCurrentIndex(tabWidget->count() - 1);
    actionSelect->trigger();
    connect(e, SIGNAL(compilationNeeded()), this, slot(slotCompilationNeeded());
}

void MainWindow::slotLoad()
{
}

void MainWindow::slotSave()
{
}

void MainWindow::slotSaveAs()
{
}

void MainWindow::slotClose()
{
    tabWidget->removeTab(tabWidget->currentIndex());
}

void MainWindow::setCurrentTool(CPNetScene::Tool tool)
{

    if(tabWidget->currentIndex() == -1)
        return;
    CPNetEditor *e = qobject_cast<CPNetEditor *>(tabWidget->currentWidget());
    e->scene->currentTool = tool;
}

void MainWindow::slotSelect()
{
    setCurrentTool(CPNetScene::SELECT);
}

void MainWindow::slotPlace()
{
    setCurrentTool(CPNetScene::PLACE);
}

void MainWindow::slotTransition()
{
    setCurrentTool(CPNetScene::TRANSITION);
}

void MainWindow::slotArc()
{
    setCurrentTool(CPNetScene::ARC);
}

void MainWindow::slotDelete()
{
    setCurrentTool(CPNetScene::DELETE);
}

void MainWindow::slotAbout()
{
}

void MainWindow::slotCompile()
{
}

void MainWindow::slotSimulate()
{
}

void MainWindow::slotTabCloseRequest(int index)
{
    tabWidget->setCurrentIndex(index);
    slotClose();
}

void MainWindow::slotCompilationNeeded()
{
    this->actionCompile->setEnabled(true);
    this->actionSimulate->setEnabled(false);
}

void MainWindow::slotCompilationDone()
{
    this->actionCompile->setEnabled(false);
    this->actionSimulate->setEnabled(true);
}

void MainWindow::slotTabChanged(int i)
{
    actionSelect->trigger();
    CPNetEditor *e = qobject_cast<CPNetEditor *> tabWidget->currentWidget();
    if(e->scene->net.isCompiled)
        slotCompilationDone();
    else
        slotCompilationDone();

}
