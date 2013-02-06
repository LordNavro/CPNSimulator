#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QLabel *l = new QLabel("test", this);
}

MainWindow::~MainWindow()
{
    
}
