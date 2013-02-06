#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>


//icons: http://www.iconfinder.com/search/?q=iconset%3Afatcow+control


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionLoad;

    QAction *actionPlace;
    QAction *actionTransition;
    QAction *actionArc;
    QAction *actionDelete;

    QAction *actionAbout;

    QToolBar *toolBarFile;
    QToolBar *toolBarTools;
    QToolBar *toolBarSimulation;

    QMenu *menuFile;
    QMenu *menuTool;
    QMenu *menuSimulation;
    QMenu *menuAbout;

    void createActions(void);
    void createToolBars(void);
    void createMenuBars(void);
};

#endif // MAINWINDOW_H
