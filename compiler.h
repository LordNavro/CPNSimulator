#ifndef COMPILER_H
#define COMPILER_H

#include <QList>
#include <QString>
#include <QPair>
#include <QMap>


    /* multiset data values */
    typedef int MultiUnit;
    struct MultiBool
    {
        int t;
        int f;
    } ;
    typedef QMap<int, int> MultiInt;

    /* data model */
    union Value
    {
        MultiUnit multiUnit;
        MultiBool multiBool;
        MultiInt *multiInt;
        bool b;
        int i;
    };

    class Data
    {
    public:
        enum Type{UNIT, BOOL, INT, MULTIUNIT, MULTIBOOL, MULTIINT};

        Data(Data::Type type = Data::UNIT);
        Data(const Data &data);
        Data &operator =(const Data &data);
        bool operator ==(const Data &data) const;
        bool operator !=(const Data &data) const;
        bool operator >(const Data &data) const;
        bool operator <(const Data &data) const;
        bool operator >=(const Data &data) const;
        bool operator <=(const Data &data) const;
        Data operator +(const Data &data) const;
        Data operator -(const Data &data) const;
        Data operator *(const Data &data) const;
        ~Data(){if(this->type == Data::MULTIINT) delete value.multiInt;}
        QString toString() const;
        Data::Type type;
        Value value;
    };

    /* forward declarations */
    class Expression;
    class Declaration;
    class Command;
    class SymbolTable;

    /* aliases */
    typedef QString Id;
    typedef QPair<Data::Type, Id> Parameter;

    typedef QList<Expression *> ExpressionList;
    typedef QList<Declaration *> DeclarationList;
    typedef QList<Command *> CommandList;
    typedef QList<Id> IdList;
    typedef QList<Parameter> ParameterList;

    /* to emulate multiple start symbols via scanner prologue */
    extern int startSymbol;
    /* to handle parsed data structures */
    extern Expression *currentParsedExpression;
    extern DeclarationList *currentParsedDeclarationList;
    /* symbol tables */
    extern SymbolTable *currentGlobalSymbolTable;
    extern SymbolTable *currentLocalSymbolTable;
    extern SymbolTable *currentSymbolTable;
    /* for function returns */
    extern Data::Type currentReturnType;
    /* to disable assignments in transition guards and postset arc expressions */
    extern bool currentAssignmentAllowed;
    /* to disable ID's in marking expressions */
    extern bool currentIdAllowed;

    /* Expressions */
    class Expression
    {
    public:
        enum Type {ASSIGN, MULTISET, AND, OR, LEQ, EQ, NEQ, GEQ, GT, LT,
                   PLUS, MINUS, MUL, DIV, MOD, NOT, UMINUS, FN, VAR, DATA,
                   CONVERT, DPLUSPRE, DPLUSPOST, DMINUSPRE, DMINUSPOST, TERNAR};

        Expression(Expression::Type type) : type(type){}
        ~Expression();
        Expression::Type type;
        Data::Type dataType;  //expression return type
        Expression *left;   //for operators
        Expression *right;   //for operators
        Expression *condition;   //for ternar operator
        Id id;  //for functions/variables
        ExpressionList *expressionList;  //for function calls
        Data *data; //for direct data
    };

    /* Declarations */

    class Declaration
    {
    public:
        enum Type {FN, VAR};

        Declaration(Declaration::Type type) : type(type){}
        ~Declaration();
        Declaration::Type type;
        Data::Type dataType;  //return type/variable type
        Id id;  //function name
        IdList idList;  //declared variables
        ParameterList parameterList;    //declared parameters
        Command *command;    //function body
    };


    /* Commands */
    class Command
    {
    public:
        enum Type {IF, IFELSE, EXPR, WHILE, DOWHILE, BLOCK, RETURN, DECL, FOR};

        Command(Command::Type type) : type(type){}
        ~Command();
        Command::Type type;
        Expression *expression;  //for branching, loops, return, simple expression
        Expression *loopInit;    //for for cycles
        Expression *loopIteration;    //for for cycles
        Data::Type dataType;  //for declarations
        IdList idList;  //for declarations
        CommandList commandList;   //for blocks
        Command *command;   //for loops/branches
        Command *command2;  //for else branch
    };

    int parseQString(QString input, int symbol);


#endif // COMPILER_H
