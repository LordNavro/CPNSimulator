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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtXmlPatterns>
#include "cpneteditor.h"
#include "cpnetsimulator.h"
#include "assistant.h"

//icons: http://www.iconfinder.com/search/?q=iconset%3Afatcow+control


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Assistant *assistant;

    QActionGroup *actionGroupTool;

    QAction *actionNew;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionClose;
    QAction *actionExport;

    QAction *actionSelect;
    QAction *actionPlace;
    QAction *actionTransition;
    QAction *actionArc;
    QAction *actionDelete;

    QAction *actionCompile;
    QAction *actionEdit;
    QAction *actionStep;
    QAction *actionStop;
    QAction *actionFastForward;
    QAction *actionFindBinding;

    QAction *actionGenerateStateSpace;

    QAction *actionAbout;
    QAction *actionHelp;

    QList<QAction *> actionsEditor;
    QList<QAction *> actionsSimulator;

    QToolBar *toolBarFile;
    QToolBar *toolBarTools;
    QToolBar *toolBarSimulation;

    QMenu *menuFile;
    QMenu *menuTool;
    QMenu *menuSimulation;
    QMenu *menuAnalysis;
    QMenu *menuHelp;

    QTabWidget *tabWidget;

    QList<CPNet *> nets;

    void createActions(void);
    void createToolBars(void);
    void createMenuBars(void);

    void setCurrentTool(EditorScene::Tool tool);

    QStackedWidget *currentStackedWidget();
    CPNetEditor *currentEditor();
    CPNetSimulator *currentSimulator();
    void refreshActions();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void slotNew();
    void slotLoad();
    void slotSave();
    void slotSaveAs();
    void slotClose();
    void slotExport();

    void slotSelect();
    void slotPlace();
    void slotTransition();
    void slotArc();
    void slotDelete();

    void slotCompile();
    void slotEdit();
    void slotStep();
    void slotStop();
    void slotFastForward();
    void slotFindBinding();

    void slotGenerateStateSpace();

    void slotHelp();
    void slotAbout();

    void slotTabCloseRequest(int index);

    void slotTabChanged(int i);

    void slotForceSelect();
};

#endif // MAINWINDOW_H
