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
