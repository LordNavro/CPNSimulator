#include "cpnet.h"
#include "symboltable.h"
#include <QtGui>

CPNet *currentParsedNet = NULL;
CPNet::ErrorItem currentParsedItem;
CPNet::ErrorReference currentParsedReference((CPNet *)NULL);
CPNet::ErrorInscription currentParsedInscription;

Expression *convert(Data::Type type, Expression *e);

CPNet::CPNet(QObject *parent) :
    QObject(parent), parsedDeclaration(NULL), globalSymbolTable(NULL)
{
}

CPNet::~CPNet()
{
    qDeleteAll(arcs);
    qDeleteAll(places);
    qDeleteAll(transitions);
    if(parsedDeclaration)
    {
        qDeleteAll(*parsedDeclaration);
        delete parsedDeclaration;
    }
    if(globalSymbolTable)
        delete globalSymbolTable;
}

void CPNet::addError(CPNet::ErrorType type, QString message)
{
    errorList.append(Error(type, currentParsedItem, currentParsedReference, currentParsedInscription, yylineno, message));
}

void CPNet::syntaxAnalysis()
{
    currentParsedNet = this;
    currentParsedItem = CPNet::NET;
    currentParsedReference = CPNet::ErrorReference(this);
    currentParsedInscription = CPNet::DECLARATION;
    errorList.clear();

    if(globalSymbolTable != NULL)
        delete globalSymbolTable;
    currentGlobalSymbolTable = globalSymbolTable = new SymbolTable();

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

        currentParsedInscription = CPNet::INITIAL;
        if(place->parsedInitialMarking)
            delete place->parsedInitialMarking;
        parseQString(place->initialMarking, START_MARKING);
        place->parsedInitialMarking = currentParsedExpression;

        currentParsedInscription = CPNet::CURRENT;
        if(place->parsedCurrentMarking)
            delete place->parsedCurrentMarking;
        parseQString(place->currentMarking, START_MARKING);
        place->parsedCurrentMarking = currentParsedExpression;
    }

    /* compile transiion guards */
    currentParsedItem = CPNet::TRANSITION;
    currentParsedInscription = CPNet::GUARD;
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
    currentParsedInscription = CPNet::EXPRESSION;
    foreach(Arc *arc, arcs)
    {
        currentParsedReference = CPNet::ErrorReference(arc);

        if(arc->parsedExpression)
            delete arc->parsedExpression;
        parseQString(arc->expression, arc->isPreset ? START_PRESET : START_EXPRESSION);
        arc->parsedExpression = currentParsedExpression;
    }
}

QList<Arc *> CPNet::presetArcs(Transition *transition)
{
    QList<Arc *> result;
    foreach(Arc *arc, arcs)
    {
        if(arc->isPreset && arc->transition == transition)
            result.append(arc);
    }
    return result;
}

QList<Arc *> CPNet::postsetArcs(Transition *transition)
{
    QList<Arc *> result;
    foreach(Arc *arc, arcs)
    {
        if(!arc->isPreset && arc->transition == transition)
            result.append(arc);
    }
    return result;
}

void CPNet::semanticAnalysis()
{
    /* Covering of variables */
    foreach(Transition *transition, transitions)
    {
        QList<Arc *> preset = presetArcs(transition);
        QList<Arc *> postset = postsetArcs(transition);
        IdList allIds;
        foreach(Arc *arc, preset)
            allIds.append(collectIds(arc->parsedExpression));
        foreach(Arc *arc, postset)
        {
            foreach(Id id, collectIds(arc->parsedExpression))
                if(!allIds.contains(id))
                    errorList.append(Error(SEMANTIC, ARC, ErrorReference(arc), EXPRESSION, 0, tr("Variable %1 not covered in preset arcs").arg(id)));
        }
        foreach(Id id, collectIds(transition->parsedGuard))
            if(!allIds.contains(id))
                errorList.append(Error(SEMANTIC, TRANSITION, ErrorReference(transition), GUARD, 0, tr("Variable %1 not covered in preset arcs").arg(id)));
    }

    /* Definition of declared functions */
    foreach(Declaration *declaration, *parsedDeclaration)
    {
        if(declaration->type == Declaration::FN && globalSymbolTable->findSymbol(declaration->id)->command == NULL)
            errorList.append(Error(SEMANTIC, NET, ErrorReference(this), DECLARATION, 0, tr("Function %1 declared, but not defined").arg(declaration->id)));
    }


    /* Place markings type */
    foreach(Place *place, places)
    {
        Data::Type dataType;
        switch(place->colourSet)
        {
        case Place::UNIT:
            dataType = Data::MULTIUNIT;
            break;
        case Place::BOOL:
            dataType = Data::MULTIBOOL;
            break;
        case Place::INT:
        default:
            dataType = Data::MULTIINT;
            break;
        }

        if(place->parsedInitialMarking)
            place->parsedInitialMarking = convert(dataType, place->parsedInitialMarking);
        if(place->parsedCurrentMarking)
            place->parsedCurrentMarking = convert(dataType, place->parsedCurrentMarking);
    }

    /* Arc expression type */
    foreach(Arc *arc, arcs)
    {
        Data::Type dataType;
        switch(arc->place->colourSet)
        {
        case Place::UNIT:
            dataType = Data::MULTIUNIT;
            break;
        case Place::BOOL:
            dataType = Data::MULTIBOOL;
            break;
        case Place::INT:
        default:
            dataType = Data::MULTIINT;
            break;
        }

        if(arc->parsedExpression)
        {
            if(arc->isPreset)
            {
                if(arc->parsedExpression->dataType != dataType)
                    addError(CPNet::SEMANTIC, "Invalid type of arc expression");
            }
            else
                arc->parsedExpression = convert(dataType, arc->parsedExpression);
        }
    }

    /* Transition guard types */
    foreach(Transition *transition, transitions)
    {
        if(transition->parsedGuard)
            transition->parsedGuard = convert(Data::BOOL, transition->parsedGuard);
    }
}

IdList CPNet::collectIds(Expression *expression)
{
    IdList result;
    if(expression == NULL)
        return result;
    switch(expression->type)
    {
    case Expression::MULTISET:
    case Expression::AND:
    case Expression::OR:
    case Expression::LEQ:
    case Expression::EQ:
    case Expression::NEQ:
    case Expression::GEQ:
    case Expression::GT:
    case Expression::LT:
    case Expression::PLUS:
    case Expression::MINUS:
    case Expression::MUL:
    case Expression::DIV:
    case Expression::MOD:
        result.append(collectIds(expression->right));
    case Expression::ASSIGN:
    case Expression::NOT:
    case Expression::UMINUS:
    case Expression::CONVERT:
        result.append(collectIds(expression->left));
        break;
    case Expression::FN:
        /*unused*/
        break;
    case Expression::DATA:
        /*no action*/
        break;
    case Expression::VAR:
        result.append(expression->id);
        break;
    }
    return result;
}

