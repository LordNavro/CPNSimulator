#include "inscriptionedit.h"

InscriptionEdit::InscriptionEdit(QWidget *parent) :
    QTextEdit(parent)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(12);

    InscriptionHighlighter *highlighter = new InscriptionHighlighter(this->document());

    QFontMetrics metrics(font);
    this->setTabStopWidth(metrics.averageCharWidth() * 2);
    this->setFont(font);
}
