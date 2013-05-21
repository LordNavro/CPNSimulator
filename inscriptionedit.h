#ifndef INSCRIPTIONEDIT_H
#define INSCRIPTIONEDIT_H

#include <QtGui>
#include "inscriptionhighlighter.h"

class InscriptionEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit InscriptionEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *e);
    void insertFromMimeData(const QMimeData *source);
signals:
    
public slots:
    
};

#endif // INSCRIPTIONEDIT_H
