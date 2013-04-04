#ifndef COMPILER_H
#define COMPILER_H

#include <QList>
#include <QString>
#include <QPair>
#include <QMap>

    /* to emulate multiple start symbols via scanner prologue */
    extern int startSymbol;

    /* multiset data values */
    typedef int MultiUnit;
    typedef struct
    {
        int t;
        int f;
    } MultiBool;
    typedef QMap<int, int> MultiInt;

    /* data model */
    typedef union
    {
        MultiUnit multiUnit;
        MultiBool multiBool;
        MultiInt *multiInt;
        bool b;
        int i;
    } Value;

    class Data
    {
    public:
        typedef enum {UNIT, BOOL, INT, MULTIUNIT, MULTIBOOL, MULTIINT} Type;

        Data(Data::Type type) : type(type){if(type == Data::MULTIINT) value.multiInt = new MultiInt;}
        ~Data(){if(this->type == Data::MULTIINT) delete value.multiInt;}
        Data::Type type;
        Value value;
    };

    /* forward declarations */
    class Expression;
    class Declaration;
    class Command;

    /* aliases */
    typedef QString Id;
    typedef QPair<Data::Type, Id> Parameter;

    typedef QList<Expression *> ExpressionList;
    typedef QList<Declaration *> DeclarationList;
    typedef QList<Command *> CommandList;
    typedef QList<Id> IdList;
    typedef QList<Parameter> ParameterList;


    /* to handle parsed data structures */
    extern Expression *parsedExpression;
    extern DeclarationList *parsedDeclaration;

    /* Expressions */
    class Expression
    {
    public:
        typedef enum {ASSIGN, MULTISET, AND, OR, LEQ, EQ, NEQ, GEQ, GT, LT,
                      PLUS, MINUS, MUL, DIV, MOD, NOT, UMINUS, FN, VAR, DATA} Type;

        Expression(Expression::Type type) : type(type){}
        ~Expression();
        Expression::Type type;
        Data::Type dataType;  //expression return type
        Expression *left;   //for operators
        Expression *right;   //for operators
        Id id;  //for functions/variables
        ExpressionList *expressionList;  //for function calls
        Data *data; //for direct data
    };

    /* Declarations */

    class Declaration
    {
    public:
        typedef enum {FN, VAR} Type;

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
        typedef enum {IF, IFELSE, EXPR, WHILE, DOWHILE, BLOCK, RETURN, DECL} Type;

        Command(Command::Type type) : type(type){}
        ~Command();
        Command::Type type;
        Expression *expression;  //for branching, loops, return, simple expression
        Data::Type dataType;  //for declarations
        IdList idList;  //for declarations
        CommandList commandList;   //for blocks
        Command *command;   //for loops/branches
        Command *command2;  //for else branch
    };

    int parseQString(QString input, int symbol);


#endif // COMPILER_H
