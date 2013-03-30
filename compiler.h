#ifndef COMPILER_H
#define COMPILER_H

    typedef QList<QString> IdList;

    typedef QList<QPair<DataType, QString> > ParameterList;

    typedef enum {UNIT, BOOL, INT, MULTIUNIT, MULTIBOOL, MULTIINT} DataType;

    typedef enum {UNITVAL, INTVAL, BOOLVAL} ExpressionType;

    typedef struct _Expression
    {
        ExpressionType expressionType;
        struct _Expression *left;
        struct _Expression *right;
        QList<struct _Expression> expressionList;
        int intVal;
        bool boolVal;
    } Expression;

    typedef QList<Expression> ExpressionList;

    typedef enum {FUNCTION, VARIABLE} DeclarationType;

    typedef struct _Declaration
    {
        DeclarationType declarationType;
        DataType dataType;
        IdList idList;
    } Declaration;

#endif // COMPILER_H
