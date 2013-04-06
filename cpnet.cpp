#include "cpnet.h"
#include <QtGui>

CPNet *currentParsedNet = NULL;
CPNet::ErrorItem currentParsedItem;
CPNet::ErrorReference currentParsedReference((CPNet *)NULL);

CPNet::CPNet(QObject *parent) :
    QObject(parent)
{
}

void CPNet::addError(CPNet::ErrorType type, QString message)
{
    errorList.append(Error(type, currentParsedItem, currentParsedReference, yylineno, message));
}

void CPNet::compile()
{
    currentParsedNet = this;
    currentParsedItem = CPNet::NET;
    currentParsedReference = CPNet::ErrorReference(this);
    this->errorList.clear();

    /* compile net itself */
    if(parsedDeclaration)
    {
        qDeleteAll(*parsedDeclaration);
        delete parsedDeclaration;
    }
    parseQString(declaration, START_DECLARATION);
    parsedDeclaration = currentParsedDeclarationList;

    /* compile place markings */
    currentParsedItem = CPNet::PLACE;
    foreach(Place *place, places)
    {
        currentParsedReference = CPNet::ErrorReference(place);

        if(place->parsedInitialMarking)
            delete place->parsedInitialMarking;
        parseQString(place->initialMarking, START_MARKING);
        place->parsedInitialMarking = currentParsedExpression;

        if(place->parsedCurrentMarking)
            delete place->parsedCurrentMarking;
        parseQString(place->currentMarking, START_MARKING);
        place->parsedCurrentMarking = currentParsedExpression;
    }

    /* compile transiion guards */
    currentParsedItem = CPNet::TRANSITION;
    foreach(Transition *transition, transitions)
    {
        currentParsedReference = CPNet::ErrorReference(transition);

        if(transition->parsedGuard)
            delete transition->parsedGuard;
        parseQString(transition->guard, START_EXPRESSION);
        transition->parsedGuard = currentParsedExpression;
    }

    /* compile arc expressions */
    currentParsedItem = CPNet::ARC;
    foreach(Arc *arc, arcs)
    {
        currentParsedReference = CPNet::ErrorReference(arc);

        if(arc->parsedExpression)
            delete arc->parsedExpression;
        parseQString(arc->expression, arc->isPreset ? START_PRESET : START_EXPRESSION);
        arc->parsedExpression = currentParsedExpression;
    }

    foreach(CPNet::Error error, errorList)
    {
        qDebug() << "ERROR" << error.message.toAscii().data();
    }
}

