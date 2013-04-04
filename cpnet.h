#ifndef CPNET_H
#define CPNET_H

#include <QObject>
#include "arc.h"
#include "compiler.h"

class CPNet : public QObject
{
    Q_OBJECT
public:
    explicit CPNet(QObject *parent = 0);

    QString name;
    QString declaration;

    QList<Place *> places;
    QList<Transition *> transitions;
    QList<Arc *> arcs;

    DeclarationList *compiledDeclaration;

    bool isCompiled;

signals:
    
public slots:
    
};

#endif // CPNET_H
