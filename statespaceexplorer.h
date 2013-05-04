#ifndef STATESPACEEXPLORER_H
#define STATESPACEEXPLORER_H

#include <QWidget>
#include "analyzer.h"

class StateSpaceExplorer : public QDialog
{
    Q_OBJECT
public:
    explicit StateSpaceExplorer(StateSpaceGraph graph, QWidget *parent = 0);

    StateSpaceGraph graph;

    QListWidget *listWidget;
    QTextEdit *textEdit;

signals:
    
public slots:
    void slotItemSelected(int i);
};

#endif // STATESPACEEXPLORER_H
