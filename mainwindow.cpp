/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    assistant = new Assistant();
    createActions();
    createToolBars();
    createMenuBars();

    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabCloseRequest(int)));
    this->setCentralWidget(tabWidget);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));

    setWindowTitle("CPNSimulator");
    setWindowIcon(QIcon(":icons/icons/diagram.ico"));
    slotNew();
    showMaximized();
    refreshActions();
}

MainWindow::~MainWindow()
{
    delete assistant;
    while(tabWidget->currentIndex() != -1)
        slotClose();
}

void MainWindow::createActions()
{
    actionNew = new QAction(style()->standardIcon(QStyle::SP_DialogResetButton), tr("&New net"), this);
    actionSave = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save net"), this);
    actionSaveAs = new QAction(QIcon(":icons/icons/save.ico"), tr("Save net as"), this);
    actionLoad = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("&Load net"), this);
    actionClose = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton), tr("&Close net"), this);
    actionExport = new QAction(QIcon(":icons/icons/pdf.ico"), tr("&Export to PDF"), this);

    actionNew->setShortcut(QKeySequence::New);
    actionSave->setShortcut(QKeySequence::Save);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    actionLoad->setShortcut(QKeySequence::Open);
    actionClose->setShortcut(QKeySequence::Close);

    connect(actionNew, SIGNAL(triggered()), this, SLOT(slotNew()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(slotSave()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(slotSaveAs()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(slotLoad()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(slotClose()));
    connect(actionExport, SIGNAL(triggered()), this, SLOT(slotExport()));

    actionsEditor << actionSave << actionSaveAs << actionExport;

    actionGroupTool = new QActionGroup(this);
    actionSelect = new QAction(QIcon(":icons/icons/cursor.ico"), tr("&Select"), actionGroupTool);
    actionPlace = new QAction(QIcon(":icons/icons/place.ico"), tr("New &place"), actionGroupTool);
    actionTransition = new QAction(QIcon(":icons/icons/transition.ico"), tr("New &transition"), actionGroupTool);
    actionArc = new QAction(QIcon(":icons/icons/arc.ico"), tr("New &arc"), actionGroupTool);
    actionDelete = new QAction(QIcon(":icons/icons/delete.ico"), tr("&Delete object"), actionGroupTool);
    actionSelect->setCheckable(true);
    actionPlace->setCheckable(true);
    actionTransition->setCheckable(true);
    actionArc->setCheckable(true);
    actionDelete->setCheckable(true);
    actionSelect->setChecked(true);

    actionSelect->setShortcut(QKeySequence("s"));
    actionPlace->setShortcut(QKeySequence("p"));
    actionTransition->setShortcut(QKeySequence("t"));
    actionArc->setShortcut(QKeySequence("a"));
    actionDelete->setShortcut(QKeySequence("d"));

    connect(actionSelect, SIGNAL(triggered()), this, SLOT(slotSelect()));
    connect(actionPlace, SIGNAL(triggered()), this, SLOT(slotPlace()));
    connect(actionTransition, SIGNAL(triggered()), this, SLOT(slotTransition()));
    connect(actionArc, SIGNAL(triggered()), this, SLOT(slotArc()));
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(slotDelete()));

    actionsEditor << actionSelect << actionPlace << actionTransition << actionArc << actionDelete;

    actionCompile = new QAction(QIcon(":icons/icons/hammer.ico"), tr("&Compile"), this);
    actionEdit = new QAction(QIcon(":icons/icons/editor.ico"), tr("&Edit net"), this);
    actionStep = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)), tr("&Step"), this);
    actionStop = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaStop)), tr("To &init marking"), this);
    actionFastForward = new QAction(QIcon(style()->standardIcon(QStyle::SP_MediaSeekForward)), tr("&Fast forward"), this);
    actionFindBinding = new QAction(QIcon(style()->standardIcon(QStyle::SP_BrowserReload)), tr("Find &binding"), this);

    actionCompile->setShortcut(QKeySequence("c"));
    actionEdit->setShortcut(QKeySequence("e"));
    actionStep->setShortcut(QKeySequence("s"));
    actionStop->setShortcut(QKeySequence("i"));
    actionFastForward->setShortcut(QKeySequence("f"));
    actionFindBinding->setShortcut(QKeySequence("b"));

    connect(actionCompile, SIGNAL(triggered()), this, SLOT(slotCompile()));
    connect(actionEdit, SIGNAL(triggered()), this, SLOT(slotEdit()));
    connect(actionStep, SIGNAL(triggered()), this, SLOT(slotStep()));
    connect(actionStop, SIGNAL(triggered()), this, SLOT(slotStop()));
    connect(actionFastForward, SIGNAL(triggered()), this, SLOT(slotFastForward()));
    connect(actionFindBinding, SIGNAL(triggered()), this, SLOT(slotFindBinding()));

    actionsEditor << actionCompile;
    actionsSimulator << actionEdit << actionStep << actionStop << actionFastForward << actionFindBinding;

    actionGenerateStateSpace = new QAction(QIcon(style()->standardIcon(QStyle::SP_CustomBase)), tr("&Generate state space"), this);

    actionGenerateStateSpace->setShortcut(QKeySequence("g"));

    connect(actionGenerateStateSpace, SIGNAL(triggered()), this, SLOT(slotGenerateStateSpace()));

    actionsSimulator << actionGenerateStateSpace;

    actionHelp = new QAction(QIcon(style()->standardIcon(QStyle::SP_DialogHelpButton)), tr("&Help"), this);
    actionAbout = new QAction(QIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation)), tr("&About program"), this);

    actionHelp->setShortcut(QKeySequence::HelpContents);

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slotHelp()));
}

void MainWindow::createToolBars()
{
    toolBarFile = addToolBar(tr("File options"));
    toolBarTools = addToolBar(tr("Tool options"));
    toolBarSimulation = addToolBar(tr("Simulation options"));

    toolBarFile->addAction(actionNew);
    toolBarFile->addAction(actionLoad);
    toolBarFile->addAction(actionSave);
    toolBarFile->addAction(actionSaveAs);
    toolBarFile->addAction(actionClose);
    toolBarFile->addSeparator();
    toolBarFile->addAction(actionExport);

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
    toolBarSimulation->addAction(actionFindBinding);
}

void MainWindow::createMenuBars()
{
    menuFile = menuBar()->addMenu(/*QIcon(style()->standardIcon(QStyle::SP_FileIcon)),*/ tr("&File"));
    menuTool = menuBar()->addMenu(/*QIcon(":/icons/icons/hammer.ico"),*/ tr("&Tool"));
    menuSimulation = menuBar()->addMenu(/*QIcon(style()->standardIcon(QStyle::SP_MediaPlay)), */tr("&Simulation"));
    menuAnalysis = menuBar()->addMenu(tr("&Analysis"));
    menuHelp = menuBar()->addMenu(/*QIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation)),*/ tr("&Help"));

    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionClose);
    menuFile->addSeparator();
    menuFile->addAction(actionExport);

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
    menuSimulation->addAction(actionFindBinding);

    menuAnalysis->addAction(actionGenerateStateSpace);

    menuHelp->addAction(actionHelp);
    menuHelp->addAction(actionAbout);
}

void MainWindow::slotNew()
{
    CPNet *net = new CPNet();
    CPNetEditor *e = new CPNetEditor(net, this);
    connect(e->scene, SIGNAL(signalForceSelect()), this, SLOT(slotForceSelect()));
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
    QFile schemaFile(":/schema/schema/savednet.xsd");
    schemaFile.open(QIODevice::ReadOnly);
    if(!schemaFile.isOpen())
    {
        QMessageBox::warning(this, "Open file error", "Failed to open file " + schemaFile.fileName());
        return;
    }
    QByteArray schemaData = schemaFile.readAll();
    QXmlSchema schema;
    schema.load(schemaData);
    if(!schema.isValid())
    {
        QMessageBox::warning(this, "Open file error", "Xml schema is not valid");
        return;
    }
    QByteArray fileData = file.readAll();
    QXmlSchemaValidator validator(schema);
    if(!validator.validate(fileData))
    {
        QMessageBox::warning(this, "Open file error", "Failed to validate " + fileName);
        return;
    }
    QDomDocument xml("ColouredPetriNet");
    if(!xml.setContent(fileData))
    {
        QMessageBox::warning(this, "Open file error", "Failed to read xml in " + fileName);
        return;
    }


    CPNet *net = new CPNet();
    CPNetEditor *e = new CPNetEditor(net, this);
    connect(e->scene, SIGNAL(signalForceSelect()), this, SLOT(slotForceSelect()));
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
    if(currentSimulator()->threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot close tab"), tr("Cannot close tab while computations still running"));
        return;
    }
    int i = tabWidget->currentIndex();
    delete nets.at(i);
    nets.removeAt(i);
    tabWidget->widget(i)->deleteLater();
    tabWidget->removeTab(i);
    refreshActions();
}

void MainWindow::slotExport()
{
    EditorScene *scene = currentEditor()->scene;
    if(scene->items().isEmpty())
    {
        QMessageBox::warning(this, tr("Export failure"), tr("The scene is empty"));
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Export to PDF", QString(), "*.pdf");
    if(fileName.isEmpty())
        return;
    QPrinter printer;
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    QRectF boundary = scene->itemsBoundingRect();

    printer.setPaperSize(boundary.size(), QPrinter::Millimeter);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter p(&printer);
    currentEditor()->scene->render(&p, QRectF(), boundary);
    p.end();
    QMessageBox::information(this, tr("Export completed"), tr("Net exported to %1").arg(fileName));
    return;
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
    {
        actionClose->setDisabled(true);
        return;
    }
    actionClose->setDisabled(false);

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    for(int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        if(currentSimulator()->threadComputer.isRunning())
        {
            event->ignore();
            QMessageBox::critical(this, tr("Closing failed"), tr("Cannot close app while computations are still running"));
            return;
        }
        event->accept();
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
    if(currentSimulator()->threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot change to edit mode"), tr("Cannot change to edit mode while computations still running"));
        return;
    }
    currentStackedWidget()->setCurrentIndex(0);
    foreach(Place *place, nets.at(tabWidget->currentIndex())->places)
    {
        place->currentMarking = place->currentMarkingValue->toString();
    }
    currentEditor()->slotSelectionChanged();
    refreshActions();
}

void MainWindow::slotStep()
{
    currentSimulator()->fireTransitions(1);
}

void MainWindow::slotStop()
{
    currentSimulator()->toInitialMarking();
}

void MainWindow::slotFastForward()
{
    bool ok;
    int count = QInputDialog::getInt(this, tr("Fast fwd"), tr("Number of transitions fired"), 20, 1, 1000, 10, &ok);
    if(ok)
        currentSimulator()->fireTransitions(count);
}

void MainWindow::slotFindBinding()
{
    currentSimulator()->findBindings();
}

void MainWindow::slotGenerateStateSpace()
{
    if(currentSimulator()->threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot analyze net"), tr("Cannot analyze net while computations still running"));
        return;
    }
    currentSimulator()->slotGenerateStateSpace();
}

void MainWindow::slotHelp()
{
    assistant->showDocumentation();
}

void MainWindow::slotAbout()
{
    QString text = tr("CPNSimulator version %1\nCopyright: %2\nContact: %3\nThis project and all of its contents are distributed under the terms of the GNU General Public License.")
            .arg("1.0", "2013, 2014 Ondrej Navratil", "xnavra23@stud.fit.vutbr.cz, nav.ondrej@gmail.com");
    QMessageBox::information(this, tr("About CPNSimulator"), text);
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

void MainWindow::slotForceSelect()
{
    actionSelect->setChecked(true);
}
