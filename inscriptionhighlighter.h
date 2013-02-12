#ifndef INSCRIPTIONHIGHLIGHTER_H
#define INSCRIPTIONHIGHLIGHTER_H

#include <QtGui>

class InscriptionHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit InscriptionHighlighter(QTextDocument *parent = 0);
    
protected:
    void highlightBlock(const QString &text);

private:
    typedef struct
    {
        QRegExp pattern;
        QTextCharFormat format;
    } HighlightRule;
    QList<HighlightRule> rules;

    typedef enum{NORMAL = 0, COMMENT = 1}BlockState;
    QRegExp commentStart;
    QRegExp commentEnd;
    QTextCharFormat commentFormat;


signals:
    
public slots:
    
};

#endif // INSCRIPTIONHIGHLIGHTER_H
