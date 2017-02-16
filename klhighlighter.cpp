/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  KLScript code highlighter for KLLibs                                   *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "klhighlighter.hpp"

KLHighlighter::KLHighlighter(QTextDocument* Parent)
: QSyntaxHighlighter(Parent)
{
	KLHighlighterRule Rule;

	Rule.Format.setForeground(Qt::darkMagenta);
	Rule.Format.setFontWeight(QFont::Normal);

	Rule.Expresion = QRegExp("\\b[0-9]+\\b");

	Rules.insert(NUMBERS, Rule);

	Rule.Format.setForeground(Qt::darkBlue);
	Rule.Format.setFontWeight(QFont::Bold);

    QFile File(":/new/prefix1/sqlwords");
    if(File.open(QIODevice::ReadOnly))
    {
        QStringList List;
        QTextStream Stream(&File);
        while(!Stream.atEnd())
        {
            QString Line;
            Line = Stream.readLine();
            if(!Line.isEmpty() && !Line.isNull())
                List.push_back(Line);
        }
        if(!List.isEmpty())
            Rule.Expresion = QRegExp(QString("\\b(?:%1)\\b").arg(List.join('|')),Qt::CaseInsensitive);

        File.close();
    }

	Rules.insert(KEYWORDS, Rule);

	Rule.Format.setForeground(Qt::darkBlue);
	Rule.Format.setFontWeight(QFont::Bold);

	Rule.Expresion = QRegExp("(?:\\~|\\+|\\-|\\*|\\/|\\%|\\^|\\=|\\!\\=|\\>|\\<|\\>\\=|\\<\\=|\\&|\\@|\\||\\?|\\(|\\)|\\;|\\,)");

	Rules.insert(OPERATORS, Rule);

}

KLHighlighter::~KLHighlighter(void) {}

void KLHighlighter::highlightBlock(const QString& Text)
{
	for (const auto& Rule: Rules)
	{
		int Index = Rule.Expresion.indexIn(Text);

		while (Index >= 0)
		{
			int Length = Rule.Expresion.matchedLength();
			setFormat(Index, Length, Rule.Format);
			Index = Rule.Expresion.indexIn(Text, Index + Length);
		}
	}
}

void KLHighlighter::SetFormat(STYLE Style, const QTextCharFormat& Format)
{
	Rules[Style].Format = Format;
}

QTextCharFormat KLHighlighter::GetFormat(STYLE Style) const
{
	return Rules[Style].Format;
}
