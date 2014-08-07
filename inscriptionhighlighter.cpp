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

#include "inscriptionhighlighter.h"

InscriptionHighlighter::InscriptionHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightRule rule;
    QTextCharFormat format;
    QStringList keywords;
    keywords << "\\bunit\\b" << "\\bnounit\\b" << "\\bbool\\b" << "\\bint\\b"
             << "\\bmultiunit\\b" << "\\bmultibool\\b" << "\\bmultiint\\b"
             << "\\bfor\\b"  << "\\bwhile\\b" << "\\bdo\\b"
             << "\\bdefault\\b" << "\\breturn\\b" << "\\bbreak\\b"
             << "\\bcontinue\\b" << "\\bif\\b" << "\\belse\\b";
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkBlue);
    foreach(QString keyword, keywords)
    {
        rule.pattern = QRegExp(keyword);
        rule.format = format;
        rules.append(rule);
    }

    format.setForeground(Qt::darkRed);
    rule.format = format;
    rule.pattern = QRegExp("true");
    rules.append(rule);
    rule.pattern = QRegExp("false");
    rules.append(rule);

    format.setFontWeight(QFont::Normal);
    format.setForeground(Qt::darkGreen);
    rule.format = format;
    rule.pattern = QRegExp("\\b[0-9]+");
    rules.append(rule);

    QStringList operators;
    operators << "\\+" << "-" << "\\*" << "/"  << "%" << "`" << "<" << ">"  << "=" << "!" << "?" << ":" << "&" << "\\|";
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkYellow);
    foreach(QString operator_, operators)
    {
        rule.pattern = QRegExp(operator_);
        rule.format = format;
        rules.append(rule);
    }

    commentFormat.setFontItalic(true);
    commentFormat.setForeground(Qt::darkGray);

    rule.format = commentFormat;
    rule.pattern = QRegExp("//[^\n]*");
    rules.append(rule);

    commentStart = QRegExp("/\\*");
    commentEnd = QRegExp("\\*/");
}

void InscriptionHighlighter::highlightBlock(const QString &text)
{
    foreach(const HighlightRule &rule, rules)
    {
        int index = rule.pattern.indexIn(text);
         while (index >= 0)
         {
             int length = rule.pattern.matchedLength();
             setFormat(index, length, rule.format);
             index = rule.pattern.indexIn(text, index + length);
         }
    }

    setCurrentBlockState(NORMAL);

    int startIndex = 0;
    if(previousBlockState() != COMMENT)
        startIndex = commentStart.indexIn(text);

    while(startIndex >= 0)
    {
        int endIndex = commentEnd.indexIn(text, startIndex);
        int commentLength;
        if(endIndex == -1)
        {
            setCurrentBlockState(COMMENT);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + commentEnd.matchedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = commentStart.indexIn(text, startIndex + commentLength);
    }
}
