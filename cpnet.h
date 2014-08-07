/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CPNET_H
#define CPNET_H

#include <QObject>
#include "arc.h"
#include "compiler.h"
#include "parser.parser.hpp"
#include "analyzer.h"

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
    ~CPNet();


    QString name;
    QString declaration;

    QList<Place *> places;
    QList<Transition *> transitions;
    QList<Arc *> arcs;

    DeclarationList *parsedDeclaration;
    QList<CPNet::Error> errorList;
    SymbolTable *globalSymbolTable;

    void addError(CPNet::ErrorType type, QString message);

    void syntaxAnalysis(void);
    void semanticAnalysis();

    IdList collectIds(Expression *expression);
    QList<Arc *> presetArcs(Transition *transition);
    QList<Arc *> postsetArcs(Transition *transition);

    NetMarking currentMarking();
    void setCurrentMarking(NetMarking marking);

signals:
    
public slots:
    
};

/*global variables for parser/scanner */
extern CPNet *currentParsedNet;
extern CPNet::ErrorItem currentParsedItem;
extern CPNet::ErrorReference currentParsedReference;
extern CPNet::ErrorInscription currentParsedInscription;

#endif // CPNET_H
