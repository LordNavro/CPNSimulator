#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createToolBars();
    createMenuBars();

    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabCloseRequest(int)));
    this->setCentralWidget(tabWidget);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));

    setWindowTitle("CPNSimulator");
    setWindowIcon(QIcon(style()->standardIcon(QStyle::SP_CommandLink)));
}

MainWindow::~MainWindow()
{
    while(tabWidget->currentIndex() != -1)
        slotClose();
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
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(slotSaveAs()));
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

    actionCompile = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)), tr("Compile"), this);

    connect(actionCompile, SIGNAL(triggered()), this, SLOT(slotCompile()));
}

void MainWindow::createToolBars()
{
    toolBarFile = addToolBar(tr("File options"));
    toolBarSimulation = addToolBar(tr("Simulation options"));
    toolBarTools = addToolBar(tr("Tool options"));
    toolBarSimulation = addToolBar(tr("Simulation options"));

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

    toolBarSimulation->addAction(actionCompile);
}

void MainWindow::createMenuBars()
{
    menuFile = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_FileIcon)), tr("File"));
    menuTool = menuBar()->addMenu(QIcon(":/icons/icons/hammer.ico"), tr("Tool"));
    menuAbout = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation)), tr("About"));
    menuSimulation = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)), tr("Simulation"));

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

    menuSimulation->addAction(actionCompile);
}

void MainWindow::slotNew()
{
    CPNet *net = new CPNet();
    CPNetEditor *e = new CPNetEditor(net, this);
    CPNetSimulator *s = new CPNetSimulator(net, e, this);
    nets.append(net);
    editors.append(e);
    simulators.append(s);

    tabWidget->addTab(e, "New net");
    tabWidget->setCurrentIndex(tabWidget->count() - 1);
    actionSelect->trigger();
}

void MainWindow::slotLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Coloured Petri Net"), QString(), "Coloured Petri Net (*.cpn)");
    if(fileName == "")
        return;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen())
    {
        QMessageBox::warning(this, "Open file error", "Failed to open file " + fileName);
        return;
    }
    QDomDocument xml("ColouredPetriNet");
    xml.setContent(file.readAll());


    CPNet *net = new CPNet();
    CPNetEditor *e = new CPNetEditor(net, this);
    CPNetSimulator *s = new CPNetSimulator(net, e, this);
    nets.append(net);
    editors.append(e);
    simulators.append(s);
    e->fileName = fileName;
    e->xmlToNet(xml);

    tabWidget->addTab(e, e->fileName);
    tabWidget->setCurrentIndex(tabWidget->count() - 1);
    actionSelect->trigger();
}

void MainWindow::slotSave()
{
    if(!tabWidget->currentWidget())
        return;
    CPNetEditor *e = qobject_cast<CPNetEditor *>(tabWidget->currentWidget());
    if(e->fileName == "")
    {
        slotSaveAs();
        return;
    }
    QFile file(e->fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Error saving net"), tr("Cant open file %1 in write mode!").arg(e->fileName));
        return;
    }
    QTextStream ts(&file);
    ts << e->netToXml().toString();
    file.close();

    QMessageBox::information(this, tr("Save net"), tr("Net %1 successfully saved as %2").arg(e->getNet()->name, e->fileName));
}

void MainWindow::slotSaveAs()
{
    if(!tabWidget->currentWidget())
        return;
    CPNetEditor *e = qobject_cast<CPNetEditor *>(tabWidget->currentWidget());
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Coloured Petri Net"), QString(), "Coloured Petri Net (*.cpn)");
    if(fileName == "")
        return;
    e->fileName = fileName;
    tabWidget->setTabText(tabWidget->currentIndex(), e->fileName);
    slotSave();
}

void MainWindow::slotClose()
{
    int i = tabWidget->currentIndex();
    delete simulators.at(i);
    delete editors.at(i);
    delete nets.at(i);
    simulators.removeAt(i);
    editors.removeAt(i);
    nets.removeAt(i);
    tabWidget->removeTab(i);
}

void MainWindow::setCurrentTool(EditorScene::Tool tool)
{

    if(tabWidget->currentIndex() == -1)
        return;
    CPNetEditor *e = qobject_cast<CPNetEditor *>(tabWidget->currentWidget());
    e->scene->currentTool = tool;
}

void MainWindow::slotSelect()
{
    setCurrentTool(EditorScene::SELECT);
}

void MainWindow::slotPlace()
{
    setCurrentTool(EditorScene::PLACE);
}

void MainWindow::slotTransition()
{
    setCurrentTool(EditorScene::TRANSITION);
}

void MainWindow::slotArc()
{
    setCurrentTool(EditorScene::ARC);
}

void MainWindow::slotDelete()
{
    setCurrentTool(EditorScene::DELETE);
}

void MainWindow::slotAbout()
{
}

void MainWindow::slotCompile()
{
    if(!tabWidget->currentWidget())
        return;
    CPNetEditor *e = qobject_cast<CPNetEditor *>(tabWidget->currentWidget());
    e->compile();
}

void MainWindow::slotSimulate()
{
}

void MainWindow::slotTabCloseRequest(int index)
{
    tabWidget->setCurrentIndex(index);
    slotClose();
}


void MainWindow::slotTabChanged(int /*i*/)
{
    actionSelect->trigger();
}
