#ifndef ARCFORM_H
#define ARCFORM_H

#include <QtGui>
#include "editorarcitem.h"
#include "inscriptionedit.h"

class ArcForm : public QWidget
{
    Q_OBJECT
private:
    EditorArcItem *arcItem;
public:
    explicit ArcForm(QWidget *parent = 0);
    
    QFormLayout *layout;
    InscriptionEdit *inscriptionExpression;

    void setArcItem(EditorArcItem *arcItem);
signals:

public slots:
    void expressionChanged();
    
};

#endif // ARCFORM_H
