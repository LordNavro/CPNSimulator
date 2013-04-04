#ifndef CPNETFORM_H
#define CPNETFORM_H

#include <QtGui>
#include "cpnet.h"
#include "inscriptionedit.h"

class CPNetForm : public QWidget
{
    Q_OBJECT

private:
    CPNet *net;
    QFormLayout *layout;
    QLineEdit *lineName;
    InscriptionEdit *inscriptionDeclaration;

public:
    explicit CPNetForm(QWidget *parent = 0);
    void setNet(CPNet *net);

signals:
    void compilationNeeded();
public slots:
    void nameChanged(QString name);
    void declarationChanged();
    
};

#endif // CPNETFORM_H
