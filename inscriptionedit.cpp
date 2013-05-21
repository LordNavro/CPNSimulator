#include "inscriptionedit.h"

InscriptionEdit::InscriptionEdit(QWidget *parent) :
    QTextEdit(parent)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(12);

    new InscriptionHighlighter(this->document());
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QSizePolicy policy = sizePolicy();
    policy.setVerticalStretch(1);
    setSizePolicy(policy);

    QFontMetrics metrics(font);
    this->setTabStopWidth(metrics.averageCharWidth() * 2);
    this->setFont(font);
}

void InscriptionEdit::keyPressEvent(QKeyEvent *e)
{
    QTextEdit::keyPressEvent(e);
    if(e->key() == Qt::Key_Return)
    {
        QTextCursor c(textCursor());
        c.movePosition(QTextCursor::Up);
        c.movePosition(QTextCursor::StartOfLine);
        QString tabs = "";
        forever{
            if(document()->toPlainText().at(c.position()) != '\t')
                break;
            tabs += '\t';
            c.movePosition(QTextCursor::Right);
        }
        insertPlainText(tabs);
    }
}

void InscriptionEdit::insertFromMimeData(const QMimeData *source)
{
    insertPlainText(source->text());
}
