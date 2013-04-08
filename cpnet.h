#ifndef CPNET_H
#define CPNET_H

#include <QObject>
#include "arc.h"
#include "compiler.h"
#include "parser.parser.hpp"

extern int yylineno;

class CPNet : public QObject
{
    Q_OBJECT
public:
    enum ErrorType {LEXICAL, SYNTACTIC, SEMANTIC};
    enum ErrorItem {PLACE, TRANSITION, ARC, NET};
    enum ErrorInscription {INITIAL, CURRENT, EXPRESSION, GUARD, DECLARATION};
    union ErrorReference
    {
        Place *place;
        Transition *transition;
        Arc *arc;
        CPNet *net;
        ErrorReference(Place *place) : place(place){}
        ErrorReference(Transition *transition) : transition(transition){}
        ErrorReference(Arc *arc) : arc(arc){}
        ErrorReference(CPNet *net) : net(net){}
    };

    struct Error
    {
        CPNet::ErrorType type;
        CPNet::ErrorItem item;
        CPNet::ErrorReference reference;
        CPNet::ErrorInscription inscription;
        int lineNo;
        QString message;
        Error(CPNet::ErrorType type, CPNet::ErrorItem item, CPNet::ErrorReference reference, CPNet::ErrorInscription inscription, int lineNo, QString message)
            : type(type), item(item), reference(reference), inscription(inscription), lineNo(lineNo), message(message){}
    };

    explicit CPNet(QObject *parent = 0);

    QString name;
    QString declaration;

    QList<Place *> places;
    QList<Transition *> transitions;
    QList<Arc *> arcs;

    DeclarationList *parsedDeclaration;
    QList<CPNet::Error> errorList;

    void addError(CPNet::ErrorType type, QString message);

    void compile(void);

signals:
    
public slots:
    
};

/*global variables for parser/scanner */
extern CPNet *currentParsedNet;
extern CPNet::ErrorItem currentParsedItem;
extern CPNet::ErrorReference currentParsedReference;
extern CPNet::ErrorInscription currentParsedInscription;

#endif // CPNET_H
