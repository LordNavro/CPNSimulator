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
    refreshActions();
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

    actionsEditor << actionSave << actionSaveAs << actionClose;

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

    actionsEditor << actionSelect << actionPlace << actionTransition << actionArc << actionDelete;

    actionAbout = new QAction(QIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation)), tr("About program"), this);

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

    actionCompile = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)), tr("Compile"), this);
    actionEdit = new QAction(QIcon(style()->standardIcon(QStyle::SP_DesktopIcon)), tr("Edit net"), this);
    actionStep = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)), tr("Step"), this);
    actionStop = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaStop)), tr("To init marking"), this);
    actionFastForward = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaSeekForward)), tr("Fast forward"), this);

    connect(actionCompile, SIGNAL(triggered()), this, SLOT(slotCompile()));
    connect(actionEdit, SIGNAL(triggered()), this, SLOT(slotEdit()));
    connect(actionStep, SIGNAL(triggered()), this, SLOT(slotStep()));
    connect(actionStop, SIGNAL(triggered()), this, SLOT(slotStop()));
    connect(actionFastForward, SIGNAL(triggered()), this, SLOT(slotFastForward()));

    actionsEditor << actionCompile;
    actionsSimulator << actionEdit << actionStep << actionStop << actionFastForward;


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
    toolBarSimulation->addAction(actionEdit);
    toolBarSimulation->addAction(actionStep);
    toolBarSimulation->addAction(actionStop);
    toolBarSimulation->addAction(actionFastForward);
}

void MainWindow::createMenuBars()
{
    menuFile = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_FileIcon)), tr("File"));
    menuTool = menuBar()->addMenu(QIcon(":/icons/icons/hammer.ico"), tr("Tool"));
    menuSimulation = menuBar()->addMenu(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)), tr("Simulation"));
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

    menuSimulation->addAction(actionCompile);
    menuSimulation->addAction(actionEdit);
    menuSimulation->addAction(actionStep);
    menuSimulation->addAction(actionStop);
    menuSimulation->addAction(actionFastForward);

    menuAbout->addAction(actionAbout);
}

void MainWindow::slotNew()
{
    CPNet *net = new CPNet();
    CPNetEditor *e = new CPNetEditor(net, this);
    CPNetSimulator *s = new CPNetSimulator(net, e, this);
    nets.append(net);
    QStackedWidget *sw = new QStackedWidget;
    sw->addWidget(e);
    sw->addWidget(s);

    tabWidget->addTab(sw, "New net");
    tabWidget->setCurrentIndex(tabWidget->count() - 1);
    //refreshActions();
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
    QStackedWidget *sw = new QStackedWidget;
    sw->addWidget(e);
    sw->addWidget(s);
    e->fileName = fileName;
    e->xmlToNet(xml);

    tabWidget->addTab(sw, e->fileName);
    tabWidget->setCurrentIndex(tabWidget->count() - 1);
    //refreshActions();
}

void MainWindow::slotSave()
{
    CPNetEditor *e = currentEditor();
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Coloured Petri Net"), QString(), "Coloured Petri Net (*.cpn)");
    if(fileName == "")
        return;
    CPNetEditor *e = currentEditor();
    e->fileName = fileName;
    tabWidget->setTabText(tabWidget->currentIndex(), e->fileName);
    slotSave();
}

void MainWindow::slotClose()
{
    int i = tabWidget->currentIndex();
    delete nets.at(i);
    nets.removeAt(i);
    delete tabWidget->widget(i);
    tabWidget->removeTab(i);
    refreshActions();
}

void MainWindow::setCurrentTool(EditorScene::Tool tool)
{
    if(tabWidget->currentIndex() == -1)
        return;
    CPNetEditor *e = currentEditor();
    e->scene->currentTool = tool;
}

QStackedWidget *MainWindow::currentStackedWidget()
{
    return qobject_cast<QStackedWidget *>(tabWidget->currentWidget());
}

CPNetEditor *MainWindow::currentEditor()
{
    return qobject_cast<CPNetEditor *>(currentStackedWidget()->widget(0));
}

CPNetSimulator *MainWindow::currentSimulator()
{
    return qobject_cast<CPNetSimulator *>(currentStackedWidget()->widget(1));
}

void MainWindow::refreshActions()
{
    foreach(QAction *action, actionsSimulator)
        action->setDisabled(true);
    foreach(QAction *action, actionsEditor)
        action->setDisabled(true);

    if(tabWidget->currentIndex() == -1)
        return;

    QStackedWidget *sw = currentStackedWidget();
    if(sw->currentIndex() == 0)
    {
        foreach(QAction *action, actionsEditor)
            action->setDisabled(false);
        actionSelect->trigger();
    }
    else
    {
        foreach(QAction *action, actionsSimulator)
            action->setDisabled(false);
    }
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
    CPNetEditor *e = currentEditor();
    int i = tabWidget->currentIndex();
    CPNet *net = nets.at(i);
    net->syntaxAnalysis();
    net->semanticAnalysis();
    e->setCompilationOutput();
    if(!net->errorList.isEmpty())
        return;

    CPNetSimulator *s = currentSimulator();
    s->loadNetGraph();
    currentStackedWidget()->setCurrentIndex(1);
    refreshActions();
}

void MainWindow::slotEdit()
{
    currentStackedWidget()->setCurrentIndex(0);
    foreach(Place *place, nets.at(tabWidget->currentIndex())->places)
    {
        place->currentMarking = place->currentMarkingValue->toString();
    }

    refreshActions();
}

void MainWindow::slotStep()
{
    currentSimulator()->fireEvents(1);
}

void MainWindow::slotStop()
{
    currentSimulator()->toInitialMarking();
}

void MainWindow::slotFastForward()
{
    bool ok;
    int count = QInputDialog::getInt(this, tr("Fast fwd"), tr("Number of events fired"), 20, 1, 1000, 10, &ok);
    if(ok)
        currentSimulator()->fireEvents(count);
}


void MainWindow::slotTabCloseRequest(int index)
{
    tabWidget->setCurrentIndex(index);
    slotClose();
}


void MainWindow::slotTabChanged(int /*i*/)
{
    refreshActions();
}
