#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "cpneteditor.h"
#include "cpnetsimulator.h"

//icons: http://www.iconfinder.com/search/?q=iconset%3Afatcow+control


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QActionGroup *actionGroupTool;

    QAction *actionNew;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionClose;

    QAction *actionSelect;
    QAction *actionPlace;
    QAction *actionTransition;
    QAction *actionArc;
    QAction *actionDelete;

    QAction *actionAbout;

    QAction *actionCompile;
    QAction *actionSimulate;

    QToolBar *toolBarFile;
    QToolBar *toolBarTools;
    QToolBar *toolBarSimulation;

    QMenu *menuFile;
    QMenu *menuTool;
    QMenu *menuSimulation;
    QMenu *menuAbout;

    QTabWidget *tabWidget;

    QList<CPNetEditor *> editors;
    QList<CPNetSimulator *> simulators;
    QList<CPNet *> nets;

    void createActions(void);
    void createToolBars(void);
    void createMenuBars(void);

    void setCurrentTool(CPNetScene::Tool tool);

public slots:
    void slotNew();
    void slotLoad();
    void slotSave();
    void slotSaveAs();
    void slotClose();

    void slotSelect();
    void slotPlace();
    void slotTransition();
    void slotArc();
    void slotDelete();

    void slotAbout();

    void slotCompile();
    void slotSimulate();

    void slotTabCloseRequest(int index);

    void slotTabChanged(int i);
};

#endif // MAINWINDOW_H
